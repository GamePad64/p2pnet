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

#ifndef PEER_H_
#define PEER_H_

#include "PeerRouteSet.h"
#include "../net/TransportSocketLink.h"
#include "../../common/crypto/PublicKeyDSA.h"
#include "TH.h"
#include <string>
#include <memory>

namespace p2pnet {
namespace peer {

class Peer {
	peer::TH m_th;
	std::unique_ptr<crypto::PublicKeyDSA> m_key_public;

	PeerRouteSet m_routeset;
public:
	typedef std::shared_ptr<Peer> pointer;

	Peer(peer::TH th);
	virtual ~Peer();

	peer::TH getTransportHash(){return m_th;}

	bool hasPublicKey(){return bool(m_key_public);};
	const crypto::PublicKeyDSA getPublicKey(){return *m_key_public;}
	void setPublicKey(crypto::PublicKeyDSA& key_public){
		crypto::PublicKeyDSA* pubkeydsa_ptr = new crypto::PublicKeyDSA(key_public);
		m_key_public = std::unique_ptr<crypto::PublicKeyDSA>(pubkeydsa_ptr);
	}

	/**
	 * RouteSet of this peer
	 */
	PeerRouteSet& getPeerRouteSet() {
		return m_routeset;
	}

	//void fromString(std::string peer_s);
	//std::string toString();
};

} /* namespace peer */
} /* namespace p2pnet */
#endif /* PEER_H_ */
