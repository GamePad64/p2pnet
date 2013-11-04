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
#include "OverlayPeer.h"
#include "OverlaySocket.h"

namespace p2pnet {
namespace overlay {

OverlayPeer::OverlayPeer(const TH& peer_th) {
	this->peer_th = peer_th;
	computeDistanceFromMe();
}

OverlayPeer::~OverlayPeer() {}

void OverlayPeer::computeDistanceFromMe(){
	distance = databases::PersonalKeyStorage::getInstance()->getMyTransportHash().computeDistance(peer_th);
}

const crypto::AES& OverlayPeer::getAESKey() const {
	return aes_key;
}

void OverlayPeer::setAESKey(const crypto::AES& aesKey) {
	aes_key = aesKey;
}

const crypto::ECDH& OverlayPeer::getECDHKey() const {
	return ecdh_key;
}

void OverlayPeer::setECDHKey(const crypto::ECDH& ecdhKey) {
	ecdh_key = ecdhKey;
}

const TH& OverlayPeer::getPeerTH() const {
	return peer_th;
}

void OverlayPeer::setPeerTH(const TH& peerTh) {
	peer_th = peerTh;
}

const crypto::PublicKeyDSA& OverlayPeer::getPublicKey() const {
	return public_key;
}

void OverlayPeer::setPublicKey(const crypto::PublicKeyDSA& publicKey) {
	public_key = publicKey;
	auto new_peer_th = overlay::TH(public_key);
	log() << "Changing " << peer_th.toBase58() << " ~~> " << new_peer_th.toBase58() << std::endl;
	log() << "Routing " << peer_th.toBase58() << " communications to " << new_peer_th.toBase58() << std::endl;
	OverlaySocket::getInstance()->m_connections.insert(std::make_pair(new_peer_th, OverlaySocket::getInstance()->m_connections.find(peer_th)->second));
	// TODO DHT k-bucket recompute
}

} /* namespace overlay */
} /* namespace p2pnet */
