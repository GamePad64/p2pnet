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

namespace p2pnet {
namespace net {

TransportSocket::~TransportSocket(){};

MessageBundle TransportSocket::createMessageBundle(const std::string message,
		TransportSocketEndpoint::pointer endpoint) {
	MessageBundle bundle;
	TransportSocketLink socket_connection(this, endpoint);

	bundle.socket_link = socket_connection;
	bundle.message = message;

	return bundle;
}

void TransportSocket::updateOnReceive(MessageBundle bundle) {
	for(auto &transportsocketlistener : m_listenerlist){
		transportsocketlistener->receivedMessage(bundle);
	}
}

void TransportSocket::updateOnSend(MessageBundle bundle) {
	for(auto &transportsocketlistener : m_listenerlist){
		transportsocketlistener->sentMessage(bundle);
	}
}

void TransportSocket::asyncReceiveFrom(
		const TransportSocketEndpoint& endpoint) {
	this->asyncReceiveFrom(endpoint.yieldCopyPtr());
}

void TransportSocket::asyncSendTo(const TransportSocketEndpoint& endpoint,
		const std::string& data) {
	this->asyncSendTo(endpoint.yieldCopyPtr(), data);
}

void TransportSocket::waitReceiveFrom(const TransportSocketEndpoint& endpoint) {
	this->waitReceiveFrom(endpoint.yieldCopyPtr());
}

void TransportSocket::waitSendTo(const TransportSocketEndpoint& endpoint,
		const std::string& data) {
	this->waitSendTo(endpoint.yieldCopyPtr(), data);
}

MessageBundle TransportSocket::hereReceiveFrom(
		TransportSocketEndpoint& endpoint) {
	return this->hereReceiveFrom(endpoint.yieldCopyPtr());
}

void TransportSocket::hereSendTo(TransportSocketEndpoint& endpoint,
		const std::string& data) {
	this->hereSendTo(endpoint.yieldCopyPtr(), data);
}

}
}
