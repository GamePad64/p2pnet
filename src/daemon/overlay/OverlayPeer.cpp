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
#include "OverlayConnection.h"
#include "../AsioIOService.h"

namespace p2pnet {
namespace overlay {

OverlayPeer::OverlayPeer(const TH& peer_th) : lost_timer(AsioIOService::getIOService()) {
	this->peer_th = peer_th;
}

OverlayPeer::~OverlayPeer() {}

void OverlayPeer::setPublicKey(const crypto::PublicKeyDSA& publicKey) {
	public_key = publicKey;
	auto new_peer_th = overlay::TH(public_key);
	log() << "Changing " << peer_th.toBase58() << " ~~> " << new_peer_th.toBase58() << std::endl;
	OverlaySocket::getInstance()->movePeer(peer_th, new_peer_th);
	// TODO DHT k-bucket recompute
}

void OverlayPeer::updateExpiryTime(boost::posix_time::ptime expiry_time) {
	if(expiry_time > expires)	// This is to prevent spoofing.
		expires = expiry_time;
}

void OverlayPeer::updateLostTime(boost::posix_time::ptime lost_time) {
	if(lost_time > lost){
		lost = lost_time;
		lost_timer.expires_at(lost);
		lost_timer.async_wait([&](boost::system::error_code ec){if(!ec){active = false;}});
	}
}

void OverlayPeer::updateEndpoint(const transport::TransportSocketEndpoint& endpoint, bool verified){
	auto it = std::find(m_tse.begin(), m_tse.end(), endpoint);

	if(it == m_tse.end())
		m_tse.erase(it);

	if(verified)
		m_tse.push_front(endpoint);
	else
		m_tse.push_back(endpoint);
}

} /* namespace overlay */
} /* namespace p2pnet */
