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
#include "UnixClientDataSocket.h"

namespace p2pnet {
namespace impl {

UnixClientDataSocket::UnixClientDataSocket(std::string socket_addr, boost::asio::io_service& io_service) : m_io_service(io_service), socket(io_service){
	socket.getSocket().connect(boost::asio::local::stream_protocol::endpoint(socket_addr));
}

UnixClientDataSocket::~UnixClientDataSocket() {}

void UnixClientDataSocket::send(api::APIMessage data, int& error_code) {
	socket.send(data, error_code);
}

api::APIMessage UnixClientDataSocket::receive(int& error_code) {
	return socket.receive(error_code);
}

void UnixClientDataSocket::asyncSend(api::APIMessage data, SendHandler send_handler) {
	socket.asyncSend(data, send_handler);
}

void UnixClientDataSocket::asyncReceive(ReceiveHandler receive_handler) {
	socket.asyncReceive(receive_handler);
}

} /* namespace impl */
} /* namespace p2pnet */
