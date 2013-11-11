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

namespace p2pnet {
namespace overlay {

OverlayDHT::OverlayDHT() : OverlayDHT(OverlaySocket::getInstance()) {}
OverlayDHT::OverlayDHT(OverlaySocket* socket_ptr) : dht::DHTService(), parent_socket_ptr(socket_ptr) {}

void OverlayDHT::send(const crypto::Hash& dest, const protocol::DHTPart& dht_part) {
	protocol::OverlayMessage_Payload payload;
	payload.SetExtension(protocol::dht_part, dht_part);
	parent_socket_ptr->send(dest, payload, protocol::OverlayMessage_Header_MessagePriority_RELIABLE);
}

void OverlayDHT::process(const crypto::Hash& from, const protocol::DHTPart& dht_part) {
}

/* K-bucket mgmt */
void OverlayDHT::registerInKBucket(std::shared_ptr<OverlayPeer> peer, unsigned short distance) {
	k_buckets[distance].insert(hash);
}
void OverlayDHT::registerInKBucket(std::shared_ptr<OverlayPeer> peer, const crypto::Hash& my_hash) {
	unsigned short distance = my_hash.computeDistance(hash);
	registerInKBucket(hash, distance);
}
void OverlayDHT::removeFromKBucket(std::shared_ptr<OverlayPeer> peer, unsigned short distance) {
	k_buckets[distance].erase(hash);
}
void OverlayDHT::removeFromKBucket(std::shared_ptr<OverlayPeer> peer, const crypto::Hash& my_hash) {
	unsigned short distance = my_hash.computeDistance(hash);
	removeFromKBucket(hash, distance);
}

OverlayDHT::~OverlayDHT() {}

} /* namespace overlay */
} /* namespace p2pnet */
