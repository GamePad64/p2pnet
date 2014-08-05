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

OverlayDHT::OverlayDHT(OverlaySocket* socket_ptr) : dht::DHTService(), parent_socket_ptr(socket_ptr) {
	rebuild();
	key_renewal = parent_socket_ptr->getKeyProvider()->getRotationSignal().connect([this](){this->rebuild();});
}
OverlayDHT::~OverlayDHT() {
	key_renewal.disconnect();
}

void OverlayDHT::send(const crypto::Hash& dest, const protocol::DHTPart& dht_part) {
	protocol::OverlayMessage_Payload payload;
	payload.MutableExtension(protocol::dht_part)->CopyFrom(dht_part);
	parent_socket_ptr->send(dest, payload, OverlaySocket::Priority::REALTIME);
}

crypto::Hash OverlayDHT::getMyHash(){
	return OverlaySocket::getInstance()->getKeyProvider()->getTH();
}

/* K-bucket mgmt */
void OverlayDHT::registerInKBucket(OverlayNode* node){
	k_buckets.addNode(node, true);
}

void OverlayDHT::removeFromKBucket(OverlayNode* node) {
	k_buckets.removeNode(node);
}

void OverlayDHT::rebuild() {
	auto hash = getMyHash();
	auto overlay_node_set = parent_socket_ptr->getNodeDB()->getAllNodes();

	std::set<dht::DHTNode*> dht_node_set(overlay_node_set.begin(), overlay_node_set.end());
	if(!k_buckets){
		k_buckets = std::unique_ptr<dht::KBucket>(new dht::KBucket(getMyHash(), k));
	}
	k_buckets.rebuild(hash, dht_node_set);
}

void OverlayDHT::foundNode(std::string serialized_contact) {
	OverlayNode node(serialized_contact);
	(*(parent_socket_ptr->getNodeDB()->getNode(node.getHash()))) = node;
}

} /* namespace overlay */
} /* namespace p2pnet */
