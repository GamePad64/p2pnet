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

#ifndef PEERROUTESET_H_
#define PEERROUTESET_H_

#include "../../common/crypto/CryptoTypes.h"
#include "../relay/RelayTransportSocketEndpoint.h"
#include "../net/udp/UDPTransportSocketEndpoint.h"
#include <list>

namespace p2pnet {
namespace peer {

/**
 * PeerRouteSet is a class that stores all the ways to reach the destination.
 */
class PeerRouteSet {
public:
	PeerRouteSet();
	virtual ~PeerRouteSet();

	std::list<relay::RelayTransportSocketEndpoint> endpoint_relay_list;
	decltype(endpoint_relay_list)& getRelayEndpointList() {return endpoint_relay_list;};
	std::list<net::UDPTransportSocketEndpoint> endpoint_udp_list;
	decltype(endpoint_udp_list)& getUDPEndpointList() {return endpoint_udp_list;}

	// Protocol Buffers serialization
	std::string toString();
	void fromString(const std::string &from);
};

} /* namespace databases */
} /* namespace peer */
#endif /* PEERROUTESET_H_ */
