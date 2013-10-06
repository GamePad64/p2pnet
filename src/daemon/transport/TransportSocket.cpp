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

#include "TransportSocket.h"
#include "TransportSocketEndpoint.h"
#include "../../common/Loggable.h"
#include <boost/range/adaptor/map.hpp>
#include <iostream>
#include <cstdlib>

namespace p2pnet {
namespace transport {

TransportSocket::~TransportSocket(){};

std::shared_ptr<TransportInterface> TransportSocket::getInterfaceByID(uint32_t id){
	return interfaces[id];
}
std::shared_ptr<TransportInterface> TransportSocket::getInterfaceByPrefix(std::string prefix) {
	return readable_strings_prefixes[prefix];
}

void TransportSocket::registerInterface(std::shared_ptr<TransportInterface> interface){
	interfaces.insert(std::make_pair(interface->getInterfaceID(), interface));
	readable_strings_prefixes.insert(std::make_pair(interface->getInterfacePrefix(), interface));
}

//void TransportSocket::updateOnReceive(TransportSocketCallback callback) {
//	for(auto &transportsocketlistener : m_listenerlist){
//		transportsocketlistener->receivedMessage(callback);
//	}
//}

void TransportSocket::receive() {
	if(interfaces.empty){
		Loggable::log("TransportSocket", Loggable::ERROR) << "P2PNet found no interfaces on your system or all of them are disabled. The program will now terminate";
		exit(-1);
	}
	for(auto& interface : interfaces | boost::adaptors::map_values){
		interface->receive();
	}
}

void TransportSocket::send(TransportSocketEndpoint endpoint, const std::string& data) {
	auto connection_it = m_connections.find(endpoint);
	if(connection_it != m_connections.end()){
		std::shared_ptr<TransportConnection> connection_ptr = connection_it->second;
		connection_ptr->send(data);
	}else{
		getInterfaceByID(endpoint.getInterfaceID())->send(endpoint, data);
	}
}

}
}
