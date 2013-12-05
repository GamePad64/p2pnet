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
	boost::asio::deadline_timer lose_timer;
	std::weak_ptr<OverlayConnection> associated_connection;	// If not, then this peer is "lost", so we will not be able to connect it again.

	void deactivate();
public:
	OverlayPeer(const TH& peer_th);
	virtual ~OverlayPeer();

	const crypto::AES& getAESKey() const;
	void setAESKey(const crypto::AES& aesKey);
	const crypto::ECDH& getECDHKey() const;
	void setECDHKey(const crypto::ECDH& ecdhKey);
	const TH& getPeerTH() const;
	void setPeerTH(const TH& peerTh);
	const crypto::PublicKeyDSA& getPublicKey() const;
	void setPublicKey(const crypto::PublicKeyDSA& publicKey);
	const boost::posix_time::ptime& getExpiryTime() const;
	void updateExpiryTime(boost::posix_time::ptime expiry_time);
	const boost::posix_time::ptime& getLostTime() const;
	void updateLostTime(boost::posix_time::ptime lost_time);

	bool isActive() const;

	std::deque<transport::TransportSocketEndpoint>& getEndpointList(){return m_tse;}
};

} /* namespace overlay */
} /* namespace p2pnet */

#endif /* OVERLAYPEER_H_ */
