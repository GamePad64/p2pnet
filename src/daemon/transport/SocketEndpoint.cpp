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
#include "Socket.h"
#include "SocketEndpoint.h"
#include "Interface.h"

#include "../../p2pnet.h"

// Standard headers
#include <memory>
#include <sstream>
#include <iostream>

namespace p2pnet {
namespace transport {

// Constructors
SocketEndpoint::SocketEndpoint(Socket* parent) : parent(parent) {}
SocketEndpoint::SocketEndpoint(const SocketEndpoint& tse) : SocketEndpoint(tse.parent, tse.interface_endpoint) {}

SocketEndpoint::SocketEndpoint(Socket* parent, const std::shared_ptr<InterfaceEndpoint> interface_endpoint) : SocketEndpoint(parent) {
	this->interface_endpoint = interface_endpoint->clone();
}

void SocketEndpoint::resetEndpoint(std::string name) {
	auto interface_ptr = parent->getInterface(name);
	if(interface_ptr == nullptr){
		throw (int)P2PError::no_such_interface;
	}
	interface_endpoint = interface_ptr->createEndpoint();
}

// Operators
SocketEndpoint& SocketEndpoint::operator =(const SocketEndpoint& tse) {
	parent = tse.parent;
	interface_endpoint = tse.interface_endpoint->clone();
	return *this;
}

bool SocketEndpoint::operator ==(const SocketEndpoint& tse) const {
	if(interface_endpoint && tse.interface_endpoint && toString() == tse.toString())
		return true;
	return interface_endpoint == tse.interface_endpoint ? true : false;
}

bool SocketEndpoint::operator <(const SocketEndpoint& tse) const {
	if(interface_endpoint && tse.interface_endpoint && toString() < tse.toString())
		return true;
	return false;
}

SocketEndpoint::operator bool() {
	return bool(interface_endpoint);
}

std::string SocketEndpoint::getPrefix() const {
	return bool(interface_endpoint) ? interface_endpoint->getPrefix() : "";
}

std::string SocketEndpoint::toString() const {
	return interface_endpoint->toString();
}

void SocketEndpoint::fromString(std::string string) {
	auto prefix = string.substr(0, string.find_first_of(delimiter));

	resetEndpoint(prefix);
	interface_endpoint->fromString(string);
}

} /* namespace transport */
} /* namespace p2pnet */
