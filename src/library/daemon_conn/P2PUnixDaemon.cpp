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
#include "../../p2pnet.h"

#ifdef BOOST_ASIO_HAS_LOCAL_SOCKETS
#include "../../common/api/UnixAPISocket.h"

#include "../P2PSession.h"

#include "P2PUnixDaemon.h"

namespace p2pnet {

P2PUnixDaemon::P2PUnixDaemon() {
	connect();
}

P2PUnixDaemon::P2PUnixDaemon(std::shared_ptr<P2PSession> session) : P2PDaemon(session) {
	connected = false;
	m_socket = nullptr;
}

P2PUnixDaemon::~P2PUnixDaemon() {
	disconnect();
}

void P2PUnixDaemon::connect() {
	m_socket = new api::unix::UnixAPISocket(session->getIOService());
	connected = false;

	auto path_list = api::unix::getSocketPathList();
	auto it = path_list.begin();
	while((!connected) && it != path_list.end()){
		try {
			m_socket_path = *it;
			m_socket->getSocket().connect(stream_protocol::endpoint(m_socket_path));
			connected = true;
		} catch (boost::system::system_error& e) {
			if(++it == path_list.end()){	// sic! increment is here.
				connected = false;
				throw std::system_error(e.code().value(), std::generic_category());	// or would it be better to throw from p2pnet::P2PErrorCategory?
			}
		}
	}

	receiveLoop();
}

void P2PUnixDaemon::connect(std::string path) {
	m_socket = new api::unix::UnixAPISocket(session->getIOService());
	connected = false;

	try {
		m_socket_path = path;
		m_socket->getSocket().connect(stream_protocol::endpoint(path));
		connected = true;
	} catch (boost::system::system_error& e) {
		connected = false;
		throw std::system_error(e.code().value(), std::generic_category());
	}

	receiveLoop();
}

bool P2PUnixDaemon::is_connected() {
	return connected;
}

void P2PUnixDaemon::asyncSend(api::APIMessage data, api::SendHandler handler) {
	m_socket->asyncSend(data, handler);
}

void P2PUnixDaemon::asyncReceive(api::ReceiveHandler handler) {
	m_socket->asyncReceive(handler);
}

int P2PUnixDaemon::disconnect() {
	delete m_socket;
	connected = false;
	return 0;
}

} /* namespace p2pnet */

#endif
