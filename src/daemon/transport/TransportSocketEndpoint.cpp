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
#include <iostream>

namespace p2pnet {
namespace transport {

// Constructors
TransportSocketEndpoint::TransportSocketEndpoint(const TransportSocketEndpoint& tse) : TransportSocketEndpoint(tse.interface_endpoint) {
}

TransportSocketEndpoint::TransportSocketEndpoint(const std::shared_ptr<TransportInterfaceEndpoint> interface_endpoint) {
	auto id = interface_endpoint->getInterfaceID();
	resetEndpointByID(id);
	//SHIT: Really dirty code. We could do this without protobuf. Just write a normal copy constructor and assigning operator.
	(this->interface_endpoint)->fromProtobuf(interface_endpoint->toProtobuf());
	//*(this->interface_endpoint) = *interface_endpoint;
}

void TransportSocketEndpoint::resetEndpointByID(uint32_t id) {
	auto interface_ptr = TransportSocket::getInstance()->getInterfaceByID(id);
	interface_endpoint = interface_ptr->createInterfaceEndpoint();
}

// Operators
void TransportSocketEndpoint::operator =(const TransportSocketEndpoint& tse) {
	auto id = tse.interface_endpoint->getInterfaceID();
	resetEndpointByID(id);
	*interface_endpoint = *(tse.interface_endpoint);
}

bool TransportSocketEndpoint::operator ==(const TransportSocketEndpoint& tse) {
	if(interface_endpoint && tse.interface_endpoint){
		if(interface_endpoint->toProtobuf().SerializeAsString() == tse.interface_endpoint->toProtobuf().SerializeAsString()){	// It is the most correct way to compare them, using protobuf.
			return true;
		}
		return false;
	}
	return interface_endpoint == tse.interface_endpoint ? true : false;
}

bool TransportSocketEndpoint::operator <(const TransportSocketEndpoint& tse) {
	if(interface_endpoint && tse.interface_endpoint){
		if(interface_endpoint->toProtobuf().SerializeAsString() < tse.interface_endpoint->toProtobuf().SerializeAsString()){	// It is not ideal, but using virtual operators is super dirty.
			return true;
		}
		return false;
	}
	return interface_endpoint == tse.interface_endpoint ? true : false;
}

TransportSocketEndpoint::operator bool() {
	return bool(interface_endpoint);
}

// Protobuf part
void TransportSocketEndpoint::fromProtobuf(proto::TransportSocketEndpoint_s tse_s){
	resetEndpointByID(tse_s.interface_id());
	interface_endpoint->fromProtobuf(tse_s);
};

proto::TransportSocketEndpoint_s TransportSocketEndpoint::toProtobuf() const {
	return interface_endpoint->toProtobuf();
}

TransportSocketEndpoint::TransportSocketEndpoint(proto::TransportSocketEndpoint_s tse_s) {
	fromProtobuf(tse_s);
}

// Binary strings part
void TransportSocketEndpoint::fromBinaryString(std::string binary_string) {
	proto::TransportSocketEndpoint_s tse_s;
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

uint32_t p2pnet::transport::TransportSocketEndpoint::getInterfaceID() const {
	return bool(interface_endpoint) ? interface_endpoint->getInterfaceID() : 0;
}

} /* namespace transport */
} /* namespace p2pnet */
