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
#include "TransportSocketConnection.h"

namespace p2pnet {
namespace net {

TransportSocketConnection::TransportSocketConnection(TransportSocket* socket, TransportSocketEndpoint::pointer endpoint) : m_socket(socket), m_endpoint_p(endpoint) {}

TransportSocketConnection::~TransportSocketConnection() {}

void TransportSocketConnection::asyncReceiveFrom() {
	m_socket->asyncReceiveFrom(m_endpoint_p);
}

void TransportSocketConnection::asyncSendTo(const std::string& data) {
	m_socket->asyncSendTo(m_endpoint_p, data);
}

void TransportSocketConnection::waitReceiveFrom() {
	m_socket->waitReceiveFrom(m_endpoint_p);
}

void TransportSocketConnection::waitSendTo(const std::string& data) {
	m_socket->waitSendTo(m_endpoint_p, data);
}

MessageBundle TransportSocketConnection::hereReceiveFrom() {
	return m_socket->hereReceiveFrom(m_endpoint_p);
}

void TransportSocketConnection::hereSendTo(const std::string& data) {
	m_socket->hereSendTo(m_endpoint_p, data);
}

} /* namespace net */
} /* namespace p2pnet */
