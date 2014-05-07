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

std::list<std::string> getSocketPathList() {
	std::list<std::string> path_list;

	auto config_instance = ConfigManager::getInstance();

	if( config_instance->getPermissions() != ConfigManager::SYSTEM && !(config_instance->getValue<bool>("api.unix.disable_user_sock")) ){
		path_list.push_back( (config_instance->getDirectory()) + (config_instance->getValue<std::string>("api.unix.user_sock_name")) );
	}
	path_list.push_back(config_instance->getValue<std::string>("api.unix.system_sock_path"));

	return path_list;
}

/* UnixSocket */
UnixAPISocket::UnixAPISocket(boost::asio::io_service& io_service) : session_socket(io_service) {}
UnixAPISocket::~UnixAPISocket() {
	session_socket.shutdown(session_socket.shutdown_both);
	session_socket.close();
}

std::error_condition UnixAPISocket::stdErrorFromBoostError(boost::system::error_code boost_error){
	return std::error_condition(boost_error.value(), std::generic_category());
}

std::string UnixAPISocket::generateSendString(std::string from){
	MESSAGE_SIZE_TYPE native_size = from.size();
	MESSAGE_SIZE_TYPE size_netlong = htonl(native_size);

	auto charlen_s = std::string((char*)(&size_netlong), MESSAGE_SIZE_LENGTH);
	return charlen_s+from;
}

stream_protocol::socket& UnixAPISocket::getSocket() {
	return session_socket;
}

void UnixAPISocket::send(api::APIMessage data, std::error_condition& error){
	auto send_string = generateSendString(data.SerializeAsString());

	boost::system::error_code boost_error;
	boost::asio::write(session_socket, boost::asio::buffer(send_string), boost_error);
	error = stdErrorFromBoostError(boost_error);
}

api::APIMessage UnixAPISocket::receive(std::error_condition& error){
	char* char_message_size = new char[MESSAGE_SIZE_LENGTH];

	boost::system::error_code boost_error;
	boost::asio::read(session_socket, boost::asio::buffer(char_message_size, MESSAGE_SIZE_LENGTH), boost_error);
	error = stdErrorFromBoostError(boost_error);

	if(boost_error){
		return api::APIMessage();
	}

	MESSAGE_SIZE_TYPE size = ntohl(*reinterpret_cast<MESSAGE_SIZE_TYPE*>(char_message_size));
	delete[] char_message_size;

	char* buffer = new char[size];
	boost::asio::read(session_socket, boost::asio::buffer(buffer, size), boost_error);
	error = stdErrorFromBoostError(boost_error);

	if(boost_error){
		return api::APIMessage();
	}

	APIMessage message_proto;
	message_proto.ParseFromArray(buffer, size);
	delete[] buffer;

	return message_proto;
}

void UnixAPISocket::asyncSend(api::APIMessage data, SendHandler send_handler) {
	auto send_string = generateSendString(data.SerializeAsString());

	boost::asio::async_write(session_socket, boost::asio::buffer(send_string),
			std::bind(&UnixAPISocket::handleSend, this, std::placeholders::_1, std::placeholders::_2, send_handler));
}

void UnixAPISocket::asyncReceive(ReceiveHandler receive_handler) {
	char* buffer = new char[MESSAGE_SIZE_LENGTH];
	boost::asio::async_read(session_socket, boost::asio::buffer(buffer, MESSAGE_SIZE_LENGTH),
			std::bind(&UnixAPISocket::handleReceiveSize, this, std::placeholders::_1, std::placeholders::_2, buffer, receive_handler));
}

void UnixAPISocket::handleSend(const boost::system::error_code& boost_error,
		std::size_t bytes_transferred,
		SendHandler send_handler) {
	session_socket.get_io_service().dispatch(std::bind(send_handler, stdErrorFromBoostError(boost_error)));
}

void UnixAPISocket::handleReceiveSize(const boost::system::error_code& boost_error,
		std::size_t bytes_transferred,
		char* char_message_size,
		ReceiveHandler receive_handler) {
	if(boost_error){
		session_socket.get_io_service().dispatch(std::bind(receive_handler, api::APIMessage(), stdErrorFromBoostError(boost_error)));
		return;
	}
	MESSAGE_SIZE_TYPE size = ntohl(*reinterpret_cast<MESSAGE_SIZE_TYPE*>(char_message_size));
	delete[] char_message_size;

	char* buffer = new char[size];
	boost::asio::async_read(session_socket, boost::asio::buffer(buffer, size),
			std::bind(&UnixAPISocket::handleReceive, this, std::placeholders::_1, buffer, std::placeholders::_2, receive_handler));
}

void UnixAPISocket::handleReceive(const boost::system::error_code& boost_error,
		char* message, uint32_t size,
		ReceiveHandler receive_handler) {
	if(boost_error){
		session_socket.get_io_service().dispatch(std::bind(receive_handler, api::APIMessage(), stdErrorFromBoostError(boost_error)));
		return;
	}
	APIMessage message_proto;
	message_proto.ParseFromArray(message, size);
	delete[] message;

	session_socket.get_io_service().dispatch(std::bind(receive_handler, message_proto, stdErrorFromBoostError(boost_error)));
}

} /* namespace unix */
} /* namespace api */
} /* namespace p2pnet */
