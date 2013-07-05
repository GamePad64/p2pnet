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
#include "TransportSocket.h"
#include "TransportSocketEndpoint.h"
#include "TransportInterface.h"
// Standard headers
#include <memory>
#include <sstream>

namespace p2pnet {
namespace net {

// Constructors
TransportSocketEndpoint::TransportSocketEndpoint(const TransportSocketEndpoint& tse) {
	*this = tse;
}

TransportSocketEndpoint::TransportSocketEndpoint(net::TransportInterfaceEndpoint::const_pointer interface_endpoint) {
	auto id = interface_endpoint->getInterfaceID();
	resetEndpointByID(id);
	*(this->interface_endpoint) = *interface_endpoint;
}

void TransportSocketEndpoint::resetEndpointByID(uint32_t id) {
	interface_endpoint = TransportSocket::getInstance()->
			getInterfaceByID(id)->
			createEndpoint();
}

// Operators
void TransportSocketEndpoint::operator =(const TransportSocketEndpoint& tse) {
	auto id = tse.interface_endpoint->getInterfaceID();
	resetEndpointByID(id);
	*interface_endpoint = *(tse.interface_endpoint);
}

// Protobuf part
void TransportSocketEndpoint::fromProtobuf(databases::TransportSocketEndpoint_s tse_s){
	resetEndpointByID(tse_s.interface_id());
	interface_endpoint->fromProtobuf(tse_s);
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
const char readable_delim = ':';

std::string TransportSocketEndpoint::toReadableString() const {
	auto interface_id = interface_endpoint->getInterfaceID();
	auto interface = TransportSocket::getInstance()->getInterfaceByID(interface_id);
	std::string readable_string = interface->getInterfacePrefix();
	readable_string += readable_delim;
	readable_string += interface_endpoint->toReadableString();
	return readable_string;
}

void TransportSocketEndpoint::fromReadableString(std::string readable_string) {
	std::stringstream ss(readable_string);
	std::string prefix, readable_part;
	std::getline(ss, prefix, readable_delim);
	std::getline(ss, readable_part);

	auto interface = TransportSocket::getInstance()->getInterfaceByPrefix(prefix);
	auto interface_id = interface->getInterfaceID();
	resetEndpointByID(interface_id);
	interface_endpoint->fromReadableString(readable_part);
}

TransportSocketEndpoint::operator bool() {
	return bool(interface_endpoint);
}

uint32_t p2pnet::net::TransportSocketEndpoint::getInterfaceID() const {
	return bool(interface_endpoint) ? interface_endpoint->getInterfaceID() : 0;
}

} /* namespace net */
} /* namespace p2pnet */

