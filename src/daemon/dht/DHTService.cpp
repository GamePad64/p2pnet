/*
 * You may redistribute this program and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "DHTService.h"
#include <algorithm>

namespace p2pnet {
namespace dht {

/* DHTClient */
DHTClient::DHTClient(DHTService* parent_service) {
	service_ptr = parent_service;
}

/* DHTService */
DHTService::DHTService() {}
DHTService::~DHTService() {}

std::list<crypto::Hash> DHTService::getClosestTo(const crypto::Hash& hash,
			unsigned short max_from_bucket,
			unsigned short max_nodes_total){
	auto hash_dist = getMyHash().computeDistance(hash);

	std::list<crypto::Hash> closest_peers;

	for(int offset = 0;
			((offset+hash_dist <= crypto::HASH_LENGTH) || (offset-hash_dist > 0)) && closest_peers.size() < max_nodes_total;
			offset++){
		int signed_offset = offset;
		do {
			if(hash_dist+signed_offset > 0 && hash_dist+signed_offset <= crypto::HASH_LENGTH){
				auto NPeersFromBucket = getNNodesFromBucket(hash_dist+signed_offset);
				for(auto it = NPeersFromBucket.begin();
						(it != NPeersFromBucket.end() || it != NPeersFromBucket.begin()+max_from_bucket) && closest_peers.size() < max_nodes_total;
						it++){
					closest_peers.push_back(*it);	// Or send find_node here
				}
			}
			signed_offset = -signed_offset;
		} while(signed_offset < 0);
	}

	return closest_peers;
}

void DHTService::findNode(const crypto::Hash& hash, DHTClient* client){
	node_queries.insert(std::make_pair(hash, client));

	auto maybe_node_info = getLocalNodeInfo(hash);
	if(maybe_node_info)
		foundNode(hash, *maybe_node_info);

	auto closest_peers = getClosestTo(hash, MAX_FROM_BUCKET, MAX_NODES_TOTAL);

	protocol::DHTPart dht_part;
	dht_part.set_ns("system");
	dht_part.set_hash(hash.toBinaryString());
	dht_part.set_message_type(dht_part.FIND_NODE);

	for(auto& it : closest_peers){
		send(it, dht_part);	// Actually, sending find_node requests to closest nodes.
	}
}

void DHTService::foundNode(const crypto::Hash& hash, std::string node_info){
	putLocalNodeInfo(hash, node_info);
	auto query_range = node_queries.equal_range(hash);

	auto it = query_range.first;
	do {
		it->second->foundNode(hash, node_info);
		node_queries.erase(it++);
	} while(it != query_range.second);
}

void DHTService::process(const crypto::Hash& from, const protocol::DHTPart& dht_part){
	if(dht_part.message_type() == dht_part.FIND_NODE){
		crypto::Hash query_hash;
		query_hash.setAsBinaryString(dht_part.hash());

		protocol::DHTPart new_dht_part;
		new_dht_part.set_ns("system");
		new_dht_part.set_hash(dht_part.hash());
		new_dht_part.set_message_type(dht_part.FIND_NODE_REPLY);

		auto closest_peers = getClosestTo(query_hash, MAX_FROM_BUCKET, MAX_NODES_TOTAL);
		for(auto& it : closest_peers){
			auto node_item_ptr = new_dht_part.add_nodes_list();
			node_item_ptr->set_hash(it.toBinaryString());
			node_item_ptr->set_node_info(*(getLocalNodeInfo(it)));
		}

		send(from, new_dht_part);
	} else if(dht_part.message_type() == dht_part.FIND_NODE_REPLY){
		for(auto it : dht_part.nodes_list()){
			crypto::Hash listed_hash;
			listed_hash.setAsBinaryString(it.hash());
			foundNode(listed_hash, it.node_info());
		}
	}
}

/* Client management */
void DHTService::registerClient(DHTClient* client_ptr) {
	clients.insert(client_ptr);
}
void DHTService::unregisterClient(DHTClient* client_ptr) {
	clients.erase(client_ptr);
	auto values_it = posted_values.begin();
	do {
		if(values_it->second.client_ptr == client_ptr)
			posted_values.erase(values_it++);
		else
			values_it++;
	} while(values_it != posted_values.end());

	auto value_queries_it = value_queries.begin();
	do {
		if(value_queries_it->second == client_ptr)
			value_queries.erase(value_queries_it++);
		else
			value_queries_it++;
	} while(value_queries_it != value_queries.end());

	auto node_queries_it = node_queries.begin();
	do {
		if(node_queries_it->second == client_ptr)
			node_queries.erase(node_queries_it++);
		else
			node_queries_it++;
	} while(node_queries_it != node_queries.end());
}

} /* namespace dht */
} /* namespace p2pnet */
