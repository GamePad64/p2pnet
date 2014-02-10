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
#include "UnixAPIClient.h"
#include <list>

namespace p2pnet {
namespace api {
namespace unix {

UnixAPIClient::UnixAPIClient(boost::asio::io_service& io_service) : m_socket(io_service) {
	connect();
}

UnixAPIClient::~UnixAPIClient() {}

void UnixAPIClient::send(api::APIMessage data, int& error_code) {
	m_socket.send(data, error_code);
}

api::APIMessage UnixAPIClient::receive(int& error_code) {
	return m_socket.receive(error_code);
}

void UnixAPIClient::asyncSend(api::APIMessage data, SendHandler send_handler){
	m_socket.asyncSend(data, send_handler);
}

void UnixAPIClient::asyncReceive(ReceiveHandler receive_handler){
	m_socket.asyncReceive(receive_handler);
}

void UnixAPIClient::connect() {
	bool connected = false;

	auto path_list = unix::getSocketPathList();
	auto it = path_list.begin();
	while((!connected) && it != path_list.end()){
		try {
			m_socket_path = *it;
			m_socket.getSocket().connect(stream_protocol::endpoint(m_socket_path));
			connected = true;
		} catch (boost::system::system_error& e) {
			if(++it == path_list.end())	// sic! increment is here.
				throw;	// We can do nothing. No socket paths are available. Maybe, p2pnetd is down?
		}	// TODO: Yep, may throw.
	}
	log() << "Connected to daemon on: " << m_socket_path << std::endl;
}

std::shared_ptr<impl::ClientDataSocket> UnixAPIClient::createDataSocket(std::string socket_addr) {
	return std::make_shared<impl::UnixClientDataSocket>(socket_addr);
}

}

} /* namespace api */
} /* namespace p2pnet */
