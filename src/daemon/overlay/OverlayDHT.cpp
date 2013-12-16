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
	parent_socket_ptr->send(dest, payload, protocol::OverlayMessage_Header_MessagePriority_RELIABLE);
}

crypto::Hash OverlayDHT::getMyHash(){
	return getMyTransportHash();
}

std::vector<crypto::Hash> OverlayDHT::getNNodesFromBucket(unsigned short bucket){
	std::vector<crypto::Hash> nodes(MAX_FROM_BUCKET);
	auto& k_bucket = k_buckets[bucket];

	int count = 0;

	for(auto it = k_bucket.begin(); it != k_bucket.end() && count++ < MAX_FROM_BUCKET; it++){
		nodes.push_back((*it)->getPeerTH());
	}
	return nodes;
}

boost::optional<std::string> OverlayDHT::getLocalNodeInfo(const crypto::Hash& hash){
	auto it = parent_socket_ptr->m_peers_conns.find(hash);
	if(it != parent_socket_ptr->m_peers_conns.end()){
		transport::proto::TransportSocketEndpointList tse_s;
		auto& tse_list = it->second.peer->getEndpointList();	// TODO: Send about expired.
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

	auto peer_ptr = parent_socket_ptr->getPeer(hash);

	for(auto& tse : tse_s.tse_s()){
		peer_ptr->updateEndpoint(tse);
	}
	registerInKBucket(peer_ptr);
}

/* K-bucket mgmt */
void OverlayDHT::registerInKBucket(std::shared_ptr<OverlayPeer> peer, unsigned short distance) {
	k_buckets[distance].insert(peer);
}
void OverlayDHT::registerInKBucket(std::shared_ptr<OverlayPeer> peer, const crypto::Hash& my_hash) {
	unsigned short distance = my_hash.computeDistance(peer->getPeerTH());
	registerInKBucket(peer, distance);
}
void OverlayDHT::registerInKBucket(std::shared_ptr<OverlayPeer> peer){
	registerInKBucket(peer, getMyHash());
}
void OverlayDHT::removeFromKBucket(std::shared_ptr<OverlayPeer> peer, unsigned short distance) {
	k_buckets[distance].erase(peer);
}
void OverlayDHT::removeFromKBucket(std::shared_ptr<OverlayPeer> peer, const crypto::Hash& my_hash) {
	unsigned short distance = my_hash.computeDistance(peer->getPeerTH());
	removeFromKBucket(peer, distance);
}
void OverlayDHT::removeFromKBucket(std::shared_ptr<OverlayPeer> peer) {
	removeFromKBucket(peer, getMyHash());
}

void OverlayDHT::keysUpdated(boost::posix_time::ptime expiry_time, boost::posix_time::ptime lose_time) {
	decltype(k_buckets) new_buckets;
	for(auto& old_bucket : k_buckets){
		for(auto& bucket_element : old_bucket){
			auto new_distance_from_me = bucket_element->getPeerTH().computeDistance(getMyTransportHash());
			new_buckets[new_distance_from_me].insert(bucket_element);
		}
	}
	std::swap(k_buckets, new_buckets);
}

OverlayDHT::~OverlayDHT() {}

} /* namespace overlay */
} /* namespace p2pnet */
