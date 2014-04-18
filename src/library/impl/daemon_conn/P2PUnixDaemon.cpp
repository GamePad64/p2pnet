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
#include "P2PUnixDaemon.h"
#include "../../../common/api/UnixAPISocket.h"

namespace p2pnet {

P2PUnixDaemon::P2PUnixDaemon() {
	m_external_io_service = false;
	m_io_service = new boost::asio::io_service();
	socket_thread = new std::thread([&]() {m_io_service->run();});

	connect();
}

P2PUnixDaemon::P2PUnixDaemon(boost::asio::io_service& io_service) {
	m_external_io_service = true;
	m_io_service = &io_service;

	connect();
}

P2PUnixDaemon::~P2PUnixDaemon() {
	if (!m_external_io_service) {
		m_io_service->stop();
		if (socket_thread->joinable())
			socket_thread->join();
		delete socket_thread;
		delete m_io_service;
	}
}

int P2PUnixDaemon::connect() {
	m_socket = new api::unix::UnixAPISocket(*m_io_service);
	bool connected = false;

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
				return -1;
			}
		}
	}
	return 0;
}

int P2PUnixDaemon::connect(std::string path) {
	m_socket = new api::unix::UnixAPISocket(*m_io_service);
	connected = false;

	try {
		m_socket_path = path;
		m_socket->getSocket().connect(stream_protocol::endpoint(path));
		connected = true;
	} catch (boost::system::system_error& e) {
		connected = false;
		return -1;
	}
	return 0;
}

bool P2PUnixDaemon::is_connected() {
	return connected;
}

void P2PUnixDaemon::send(api::APIMessage data, int& error_code) {
	m_socket->send(data, error_code);
}

api::APIMessage P2PUnixDaemon::receive(int& error_code) {
	auto message = m_socket->receive(error_code);
	return message;
}

int P2PUnixDaemon::disconnect() {
	delete m_socket;
	connected = false;
	return 0;
}

} /* namespace p2pnet */

#endif
