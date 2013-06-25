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

TransportInterface* TransportSocketEndpoint::getInterfaceByID(uint32_t id) {
	return TransportSocket::getInstance()->getInterfaceByID(id);
}

// Protobuf part
void TransportSocketEndpoint::fromProtobuf(databases::TransportSocketEndpoint_s tse_s){
	uint32_t iface_id = tse_s;
	auto new_interface_endpoint = getInterfaceByID(iface_id)->createEndpoint();
	new_interface_endpoint->fromProtobuf(tse_s);

	interface_endpoint = new_interface_endpoint;
};

databases::TransportSocketEndpoint_s TransportSocketEndpoint::toProtobuf() const {
	return interface_endpoint->toProtobuf();
}

TransportSocketEndpoint::TransportSocketEndpoint(databases::TransportSocketEndpoint_s tse_s) {
	fromProtobuf(tse_s);
}

// Binary strings part
void TransportSocketEndpoint::fromBinaryString(std::string binary_string) {
	databases::TransportSocketEndpoint_s tse_s;
	tse_s.ParseFromString(binary_string);
	fromProtobuf(tse_s);
}

// Readable strings part
// Well, a complicated part. It is complicated not because string concatenation :D, but
// because of concept. We must keep in mind that human readable strings are generated here.
std::string TransportSocketEndpoint::toReadableString() const {
	unsigned int iface_id = interface_endpoint->getInterfaceID();
	TransportInterface* iface = getInterfaceByID(iface_id);
}

void TransportSocketEndpoint::fromReadableString(std::string readable_string) const {
}

} /* namespace net */
} /* namespace p2pnet */
