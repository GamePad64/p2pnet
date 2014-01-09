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

namespace p2pnet {
namespace api {

/* UnixAPISession */

UnixAPISession::UnixAPISession(boost::asio::io_service& io_service) : session_socket(io_service) {
}

UnixAPISession::~UnixAPISession() {
}

stream_protocol::socket& UnixAPISession::getSocket() {
	return session_socket;
}

void UnixAPISession::startReceive() {
	char* buffer = new char[4];
	boost::asio::async_read(session_socket, boost::asio::buffer(buffer, 4),
			std::bind(&UnixAPISession::handleReceiveSize, this, buffer));
}

void UnixAPISession::handleReceiveSize(char* char_message_size) {
	MESSAGE_SIZE_TYPE size = ntohl(*reinterpret_cast<MESSAGE_SIZE_TYPE*>(char_message_size));
	delete[] char_message_size;

	char* buffer = new char[size];
	boost::asio::async_read(session_socket, boost::asio::buffer(buffer, MESSAGE_SIZE_LENGTH),
			std::bind(&UnixAPISession::handleReceive, this, buffer, std::placeholders::_2));
}

void UnixAPISession::handleReceive(char* message, uint32_t size) {
	std::string message_s(message, size);
	delete[] message;
	process(message_s);
}

void UnixAPISession::send(std::string message) {
	MESSAGE_SIZE_TYPE size_netlong = htonl(message.length());
	auto charlen_s = std::string(reinterpret_cast<char*>(&size_netlong), MESSAGE_SIZE_LENGTH);
	boost::asio::async_write(session_socket, boost::asio::buffer(charlen_s+message),
				std::bind(&UnixAPISession::handleSend, this));
}

void UnixAPISession::handleSend() {}

/* UnixAPIServer */

UnixAPIServer::UnixAPIServer(boost::asio::io_service& io_service) :
		asio_io_service(io_service) {
	socket_path = getSocketPath();

	try {
		// Try default path (i.e. that, from config: api.unix.system_sock)
		acceptor_ptr = std::unique_ptr<stream_protocol::acceptor>(new stream_protocol::acceptor(asio_io_service, stream_protocol::endpoint(socket_path)));
	} catch(boost::system::system_error& e){
		socket_path = getFallbackSocketPath();
		acceptor_ptr = std::unique_ptr<stream_protocol::acceptor>(new stream_protocol::acceptor(asio_io_service, stream_protocol::endpoint(socket_path)));
	}
	log() << "Unix API initialized at: " << socket_path << std::endl;
	accept();
}

UnixAPIServer::~UnixAPIServer() {}

void UnixAPIServer::accept() {
	auto new_session = std::make_shared<UnixAPISession>(asio_io_service);
	unix_sessions.insert(new_session);
	acceptor_ptr->async_accept(new_session->getSocket(), std::bind(&UnixAPISession::startReceive, new_session));
}

void UnixAPIServer::handleAccept(std::shared_ptr<UnixAPISession> new_session) {
	new_session->startReceive();
	accept();
}

std::string UnixAPIServer::getSocketPath() const {
	return getValue<std::string>("api.unix.system_sock");
}

std::string UnixAPIServer::getFallbackSocketPath() const {
	return ConfigManager::getInstance()->getDirectory()+"unix_api.sock";
}

} /* namespace api */
} /* namespace p2pnet */
