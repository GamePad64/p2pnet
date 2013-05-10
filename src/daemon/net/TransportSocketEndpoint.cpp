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

// Base class
#include "TransportSocketEndpoint.h"
// Child classes
#include "udp/UDPTransportSocketEndpoint.h"
#include "../relay/RelayTransportSocketEndpoint.h"
// Standard headers
#include <memory>

namespace p2pnet {
namespace net {

TransportSocketEndpoint::pointer TransportSocketEndpoint::fromProtobuf(databases::TransportSocketEndpoint_s tse_s){
	TransportSocketEndpoint::pointer tse_ptr;
	switch(tse_s.type()){
	case databases::TransportSocketEndpoint_s::UDP:
		tse_ptr = std::make_shared<UDPTransportSocketEndpoint>(tse_s);
		break;
	case databases::TransportSocketEndpoint_s::RELAY:
		tse_ptr = std::make_shared<relay::RelayTransportSocketEndpoint>(tse_s);
		break;
	default:
		;
	}
	return tse_ptr;
};

TransportSocketEndpoint::pointer TransportSocketEndpoint::fromString(std::string endpoint_s){
	databases::TransportSocketEndpoint_s tse_s;
	tse_s.ParseFromString(endpoint_s);
	return fromProtobuf(tse_s);
};

} /* namespace net */
} /* namespace p2pnet */
