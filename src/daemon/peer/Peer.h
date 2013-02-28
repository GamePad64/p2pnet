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
#include <string>

namespace p2pnet {
namespace peer {

class Peer {
public:
	Peer();
	virtual ~Peer();

	PeerRouteSet m_routeset;
	PeerRouteSet& getPeerRouteSet(){return m_routeset;};

	net::TransportSocketLink m_link;
	net::TransportSocketLink& getLink();

	//void fromString(std::string peer_s);
	//std::string toString();
};

} /* namespace peer */
} /* namespace p2pnet */
#endif /* PEER_H_ */
