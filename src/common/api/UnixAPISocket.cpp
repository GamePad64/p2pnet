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
#include "UnixAPISocket.h"
#include "../Config.h"

namespace p2pnet {
namespace api {
namespace unix {

std::string getSocketPath() {
	return ConfigManager::getInstance()->getValue<std::string>("api.unix.system_sock");
}

std::string getFallbackSocketPath() {
	return ConfigManager::getInstance()->getDirectory()+"unix_api.sock";
}

/* UnixSocket */
UnixAPISocket::UnixAPISocket(boost::asio::io_service& io_service, LowLevelAPISession* session) : session_socket(io_service) {
	session_ptr = session;
}
UnixAPISocket::~UnixAPISocket() {}

stream_protocol::socket& UnixAPISocket::getSocket() {
	return session_socket;
}

void UnixAPISocket::startReceive() {
	char* buffer = new char[MESSAGE_SIZE_LENGTH];
	boost::asio::async_read(session_socket, boost::asio::buffer(buffer, MESSAGE_SIZE_LENGTH),
			std::bind(&UnixAPISocket::handleReceiveSize, this, std::placeholders::_1, buffer));
}

void UnixAPISocket::handleReceiveSize(const boost::system::error_code& error, char* char_message_size) {
	if(error){
		session_ptr->shutdown();
		return;
	}
	MESSAGE_SIZE_TYPE size = ntohl(*reinterpret_cast<MESSAGE_SIZE_TYPE*>(char_message_size));
	delete[] char_message_size;

	char* buffer = new char[size];
	boost::asio::async_read(session_socket, boost::asio::buffer(buffer, size),
			std::bind(&UnixAPISocket::handleReceive, this, std::placeholders::_1, buffer, std::placeholders::_2));
}

void UnixAPISocket::handleReceive(const boost::system::error_code& error, char* message, uint32_t size) {
	if(error){
		session_ptr->shutdown();
		return;
	}
	APIMessage message_proto;
	message_proto.ParseFromArray(message, size);
	delete[] message;
	session_ptr->process(message_proto);
	startReceive();
}

void UnixAPISocket::send(APIMessage message) {
	auto native_size = message.ByteSize();
	MESSAGE_SIZE_TYPE size_netlong = htonl(native_size);

	std::cout << size_netlong;

	auto charlen_s = std::string(reinterpret_cast<char*>(&size_netlong), MESSAGE_SIZE_LENGTH);
	boost::asio::async_write(session_socket, boost::asio::buffer(charlen_s+message.SerializeAsString()),
				std::bind(&UnixAPISocket::handleSend, this, std::placeholders::_1));
}

void UnixAPISocket::handleSend(const boost::system::error_code& error) {
	if(error){
		session_ptr->shutdown();
	}
}

} /* namespace unix */
} /* namespace api */
} /* namespace p2pnet */
