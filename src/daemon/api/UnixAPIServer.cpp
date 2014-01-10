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
#include "UnixAPIServer.h"
#include <boost/lexical_cast.hpp>
#include <cstdio>

namespace p2pnet {
namespace api {
namespace unix {

/* UnixAPISession */

UnixAPISession::UnixAPISession(boost::asio::io_service& io_service, UnixAPIServer* parent) : socket(io_service, this) {
	parent_apiserver = parent;
}

UnixAPISession::~UnixAPISession() {
}

void UnixAPISession::send(APIMessage message) {
	socket.send(message);
}

void UnixAPISession::shutdown(){
	parent_apiserver->unix_sessions.erase(shared_from_this());
}

/* UnixAPIServer */

UnixAPIServer::UnixAPIServer(boost::asio::io_service& io_service) :
		asio_io_service(io_service) {
	try {
		socket_path = getSocketPath();	// Try default path (i.e. that, from config: api.unix.system_sock)
		std::remove(socket_path.c_str());
		acceptor_ptr = std::unique_ptr<stream_protocol::acceptor>(new stream_protocol::acceptor(asio_io_service, stream_protocol::endpoint(socket_path)));
	} catch(boost::system::system_error& e){
		socket_path = getFallbackSocketPath();
		std::remove(socket_path.c_str());
		acceptor_ptr = std::unique_ptr<stream_protocol::acceptor>(new stream_protocol::acceptor(asio_io_service, stream_protocol::endpoint(socket_path)));
	}
	log() << "Unix API initialized at: " << socket_path << std::endl;
	accept();
}

UnixAPIServer::~UnixAPIServer() {}

void UnixAPIServer::accept() {
	auto new_session = std::make_shared<UnixAPISession>(asio_io_service, this);
	unix_sessions.insert(new_session);
	acceptor_ptr->async_accept(new_session->getUnixSocket().getSocket(), std::bind(&UnixAPIServer::handleAccept, this, new_session));
}

void UnixAPIServer::handleAccept(std::shared_ptr<UnixAPISession> new_session) {
	new_session->getUnixSocket().startReceive();
	accept();
}

} /* namespace unix */
} /* namespace api */
} /* namespace p2pnet */
