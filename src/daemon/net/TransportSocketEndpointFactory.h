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

#ifndef TRANSPORTSOCKETENDPOINTFACTORY_H_
#define TRANSPORTSOCKETENDPOINTFACTORY_H_

#include "../protobuf/TransportSocketEndpoint_s.pb.h"
#include "TransportSocketEndpoint.h"
#include "udp/UDPTransportSocketEndpoint.h"
#include "../relay/RelayTransportSocketEndpoint.h"

namespace p2pnet {
namespace net {

class TransportSocketEndpointFactory {
public:
	TransportSocketEndpointFactory();
	virtual ~TransportSocketEndpointFactory();

	TransportSocketEndpoint::pointer createEndpointFromProtobuf(TransportSocketEndpoint_s tse_s);
	TransportSocketEndpoint::pointer createEndpointFromString(std::string tse_str);
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* TRANSPORTSOCKETENDPOINTFACTORY_H_ */
