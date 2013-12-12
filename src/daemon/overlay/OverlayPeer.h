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
#ifndef OVERLAYPEER_H_
#define OVERLAYPEER_H_

#include "../transport/TransportSocketEndpoint.h"
#include "TH.h"
#include "../../common/Loggable.h"
#include "../databases/PersonalKeyStorage.h"
#include "../../common/crypto/ECDH.h"
#include "../../common/crypto/AES.h"
#include "../../common/crypto/PublicKeyDSA.h"
#include <memory>

namespace p2pnet {
namespace overlay {

class OverlayConnection;

class OverlayPeer : Loggable {
	/* Various peer information */
	TH peer_th;

	crypto::PublicKeyDSA public_key;
	crypto::ECDH ecdh_key;
	crypto::AES aes_key;

	boost::posix_time::ptime expires;
	boost::posix_time::ptime lost;

	std::deque<transport::TransportSocketEndpoint> m_tse;

	/* Other stuff */
	boost::asio::deadline_timer lost_timer;
	bool active = true;	// It means "not expired". If expired, then connection associated with this peer will be wiped.
public:
	OverlayPeer(const TH& peer_th);
	virtual ~OverlayPeer();

	const crypto::AES& getAESKey() const {return aes_key;};
	void setAESKey(const crypto::AES& aesKey) {aes_key = aesKey;};
	const crypto::ECDH& getECDHKey() const {return ecdh_key;};
	void setECDHKey(const crypto::ECDH& ecdhKey) {ecdh_key = ecdhKey;};
	const TH& getPeerTH() const {return peer_th;};
	void setPeerTH(const TH& peerTh) {peer_th = peerTh;};
	const crypto::PublicKeyDSA& getPublicKey() const {return public_key;};
	void setPublicKey(const crypto::PublicKeyDSA& publicKey);

	const boost::posix_time::ptime& getExpiryTime() const {return expires;};
	void updateExpiryTime(boost::posix_time::ptime expiry_time);
	const boost::posix_time::ptime& getLostTime() const {return lost;};
	void updateLostTime(boost::posix_time::ptime lost_time);

	/**
	 * This function adds a new TSE to this peer or "pops out" old TSE.
	 * @param endpoint
	 * @param verified
	 */
	void updateEndpoint(const transport::TransportSocketEndpoint& endpoint, bool verified = false);
	const std::deque<transport::TransportSocketEndpoint>& getEndpointList() const {return m_tse;};

	bool isActive() const {return active;};
};

} /* namespace overlay */
} /* namespace p2pnet */

#endif /* OVERLAYPEER_H_ */
