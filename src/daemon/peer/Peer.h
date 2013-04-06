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
#include "../../common/crypto/CurrentCipherSet.h"
#include <string>

namespace p2pnet {
namespace peer {

class Peer {
	crypto::Hash m_transport_hash;
	crypto::key_public_t m_key_public;

	PeerRouteSet m_routeset;
public:
	Peer();
	virtual ~Peer();

	crypto::Hash getTransportHash(){return m_transport_hash;}
	void setTransportHash(crypto::Hash transport_hash){
		m_transport_hash = transport_hash;
	}

	crypto::key_public_t getPublicKey(){return m_key_public;}
	void setPublicKey(crypto::key_public_t key_public){
		m_key_public = key_public;
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
