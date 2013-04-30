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

#include "PeerRouteSet.h"
#include "../protobuf/TransportSocketEndpoint_s.pb.h"

namespace p2pnet {
namespace peer {

PeerRouteSet::PeerRouteSet() {}
PeerRouteSet::~PeerRouteSet() {}

std::string PeerRouteSet::toString() {
	net::PeerRoute_s peerroute_s;
	for(net::TransportSocketEndpoint::pointer &endpoint_relay : this->endpoint_relay_list){
		peerroute_s.add_endpoint_relay(endpoint_relay->toString());
	}
	for(net::TransportSocketEndpoint::pointer &endpoint_udp : this->endpoint_udp_list){
		peerroute_s.add_endpoint_udp(endpoint_udp->toString());
	}
	return peerroute_s.SerializeAsString();
}

void PeerRouteSet::fromString(const std::string &from) {
	net::PeerRoute_s peerroute_s;
	peerroute_s.ParseFromString(from);

	for(const std::string &endpoint_relay_s : peerroute_s.endpoint_relay()){
		this->endpoint_relay_list.push_front(net::TransportSocketEndpoint::fromString(endpoint_relay_s));
	}
	for(const std::string &endpoint_udp_s : peerroute_s.endpoint_udp()){
		this->endpoint_relay_list.push_front(net::TransportSocketEndpoint::fromString(endpoint_udp_s));
	}
}

} /* namespace peer */
} /* namespace p2pnet */
