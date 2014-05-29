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
#include "OverlayDHT.h"
#include "OverlaySocket.h"
#include "OverlayConnection.h"
#include "../transport/TransportSocketEndpoint.h"

namespace p2pnet {
namespace overlay {

OverlayDHT::OverlayDHT(OverlaySocket* socket_ptr) : dht::DHTService(), parent_socket_ptr(socket_ptr) {}

void OverlayDHT::send(const crypto::Hash& dest, const protocol::DHTPart& dht_part) {
	protocol::OverlayMessage_Payload payload;
	payload.MutableExtension(protocol::dht_part)->CopyFrom(dht_part);
	parent_socket_ptr->send(dest, payload, OverlaySocket::Priority::RELIABLE);
}

crypto::Hash OverlayDHT::getMyHash(){
	return OverlaySocket::getInstance()->getKeyProvider()->getTH();
}

std::vector<crypto::Hash> OverlayDHT::getNNodesFromBucket(unsigned short bucket){
	std::vector<crypto::Hash>  nodes(MAX_FROM_BUCKET);
	auto& k_bucket = k_buckets[bucket];

	int count = 0;

	for(auto it = k_bucket.begin(); it != k_bucket.end() && count++ < MAX_FROM_BUCKET; it++){
		nodes.push_back((*it)->remoteTH());
	}
	return nodes;
}

boost::optional<std::string> OverlayDHT::getLocalNodeInfo(const crypto::Hash& hash){
	auto it = parent_socket_ptr->m_connections.find(hash);
	if(it != parent_socket_ptr->m_connections.end()){
		transport::proto::TransportSocketEndpointList tse_s;
		const auto& tse_list = it->second->getTransportEndpoints();	// TODO: Send about expired.
		for(auto tse_it : tse_list){
			tse_s.add_tse_s()->CopyFrom(tse_it.toProtobuf());
		}
		return boost::optional<std::string>(tse_s.SerializeAsString());
	}
	return boost::optional<std::string>();
}

void OverlayDHT::putLocalNodeInfo(const crypto::Hash& hash, std::string node_info){
	transport::proto::TransportSocketEndpointList tse_s;
	tse_s.ParseFromString(node_info);

	auto peer_ptr = parent_socket_ptr->getConnection(hash);

	for(auto& tse : tse_s.tse_s()){
		peer_ptr->updateEndpoint(tse);
	}
	registerInKBucket(peer_ptr);
}

/* K-bucket mgmt */
void OverlayDHT::registerInKBucket(OverlayConnection* peer, unsigned short distance) {
	k_buckets[distance].insert(peer);
}
void OverlayDHT::registerInKBucket(OverlayConnection* peer, const crypto::Hash& my_hash) {
	unsigned short distance = my_hash.computeDistance(peer->remoteTH());
	registerInKBucket(peer, distance);
}
void OverlayDHT::registerInKBucket(OverlayConnection* peer){
	registerInKBucket(peer, getMyHash());
}
void OverlayDHT::removeFromKBucket(OverlayConnection* peer, unsigned short distance) {
	k_buckets[distance].erase(peer);
}
void OverlayDHT::removeFromKBucket(OverlayConnection* peer, const crypto::Hash& my_hash) {
	unsigned short distance = my_hash.computeDistance(peer->remoteTH());
	removeFromKBucket(peer, distance);
}
void OverlayDHT::removeFromKBucket(OverlayConnection* peer) {
	removeFromKBucket(peer, getMyHash());
}

void OverlayDHT::recomputeAll() {
	decltype(k_buckets) new_buckets;
	for(auto& old_bucket : k_buckets){
		for(auto& bucket_element : old_bucket){
			auto new_distance_from_me = bucket_element->remoteTH().computeDistance(getMyHash());
			new_buckets[new_distance_from_me].insert(bucket_element);
		}
	}
	log() << "Recomputing DHT" << std::endl;
	std::swap(k_buckets, new_buckets);
}

OverlayDHT::~OverlayDHT() {}

} /* namespace overlay */
} /* namespace p2pnet */
