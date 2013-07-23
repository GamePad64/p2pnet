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
#include "TransportSocketListener.h"
#include "TransportSocketEndpoint.h"
#include <iostream>

namespace p2pnet {
namespace net {

TransportSocket::~TransportSocket(){};

TransportInterface* TransportSocket::getInterfaceByID(uint32_t id){
	return interfaces[id];
}
TransportInterface* TransportSocket::getInterfaceByPrefix(std::string prefix) {
	return readable_strings_prefixes[prefix];
}

void TransportSocket::registerInterface(TransportInterface* interface){
	auto interface_id = interface->getInterfaceID();
	interfaces[interface_id] = interface;
	readable_strings_prefixes[interface->getInterfacePrefix()] = interface;
}

void TransportSocket::updateOnReceive(TransportSocketCallback callback) {
	for(auto &transportsocketlistener : m_listenerlist){
		transportsocketlistener->receivedMessage(callback);
	}
}

void TransportSocket::updateOnSend(TransportSocketCallback callback) {
	for(auto &transportsocketlistener : m_listenerlist){
		transportsocketlistener->sentMessage(callback);
	}
}

void TransportSocket::asyncReceiveFrom(TransportSocketEndpoint endpoint) {
}

void TransportSocket::waitReceiveFrom(TransportSocketEndpoint endpoint) {
}

TransportSocketCallback TransportSocket::hereReceiveFrom(TransportSocketEndpoint endpoint) {
}

void TransportSocket::asyncSendTo(TransportSocketEndpoint endpoint, const std::string& data) {
	getInterfaceByID(endpoint.getInterfaceID())->asyncSendTo(endpoint.interface_endpoint, data);
}

void TransportSocket::waitSendTo(TransportSocketEndpoint endpoint, const std::string& data) {
}

void TransportSocket::hereSendTo(TransportSocketEndpoint endpoint, const std::string& data) {
}

}
}
