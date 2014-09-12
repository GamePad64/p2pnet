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

#include "Socket.h"
#include "Interface.h"
#include "SocketEndpoint.h"
#include "Connection.h"
#include "../../common/Loggable.h"
#include "../errors/NoInterface.h"
#include <boost/range/adaptor/map.hpp>
#include <iostream>
#include <cstdlib>

namespace p2pnet {
namespace transport {

Socket::Socket(){
	log() << "Initialized" << std::endl;
}
Socket::~Socket(){};

std::shared_ptr<Interface> Socket::getInterface(std::string prefix) {
	auto it = interfaces.find(prefix);
	if(it == interfaces.end()){
		return nullptr;
	}
	return it->second;
}

void Socket::registerInterface(std::shared_ptr<Interface> interface){
	interfaces.insert(std::make_pair(interface->getPrefix(), interface));
	interface->parent_socket = shared_from_this();
	log() << "Registered interface: " << interface->getPrefix() << std::endl;
}

void Socket::setReceiveCallback(ReceiveCallback callback) {
	for(auto interface : boost::adaptors::values(interfaces)){
		interface->setReceiveCallback(callback);
	}
}

void Socket::setDisconnectCallback(DisconnectCallback callback) {
	for(auto interface : boost::adaptors::values(interfaces)){
		interface->setDisconnectCallback(callback);
	}
}

std::shared_ptr<Connection> Socket::getConnection(SocketEndpoint endpoint) {
	auto it = connections.find(endpoint);
	return it != connections.end() ? it->second : nullptr;
}

void Socket::registerConnection(std::shared_ptr<Connection> connection){
	connections.insert(std::make_pair(connection->getEndpoint(), connection));
}
void Socket::unregisterConnection(std::shared_ptr<Connection> connection){
	connections.erase(connection->getEndpoint());
}

void Socket::connect(SocketEndpoint endpoint, ConnectCallback callback) {
	interfaces[endpoint.getPrefix()]->connect(endpoint, callback);
}

}
}
