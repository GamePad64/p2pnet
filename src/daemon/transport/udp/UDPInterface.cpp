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

#include "UDPInterface.h"
#include "UDPConnection.h"
#include "../SocketEndpoint.h"
#include "../../AsioIOService.h"
#include <boost/bind.hpp>
#include <string>
#include <memory>

namespace p2pnet {
namespace transport {

UDPInterface::UDPInterface() : m_socket(AsioIOService::getIOService()) {
	local.address(address::from_string(getValue<std::string>("transport.udp.local_ip")));
	local.port(getValue<unsigned short>("transport.udp.port"));

	mtu = getValue<unsigned short>("transport.udp.mtu");
	keepalive_timer = getValue<int>("transport.udp.keepalive_timer");
	inactivity_timeout = getValue<int>("transport.udp.inactivity_timeout");

	m_socket.open(udp::endpoint::protocol_type::v6());
	m_socket.bind(local);
}

UDPInterface::~UDPInterface() {
	if (m_socket.is_open())
		m_socket.close();
}

void UDPInterface::handleReceive(const boost::system::error_code& error, charbuf_ptr_t buffer,
		size_t bytes_received,
		std::shared_ptr<udp::endpoint> endpoint) {
	std::string message(buffer->data(), bytes_received);
	SocketEndpoint socket_endpoint(getParent().get(), std::make_shared<UDPInterfaceEndpoint>(endpoint->address(), endpoint->port()));

	auto connection = std::static_pointer_cast<UDPConnection>(getParent()->getConnection(socket_endpoint));
	if(connection){
		if(message == UDP_KEEPALIVE_MSG){
			connection->receivedKeepAlive();
		}else if(message == UDP_DISCONNECT_MSG){
			connection->receivedDisconnect();
		}else{
			m_socket.get_io_service().dispatch(std::bind(receive_callback, error.value(), connection, message));
		}
	}else if(message == UDP_CONNECT_MSG && !getValue<bool>("transport.udp.outgoing_only")){
		auto interface_endpoint = std::make_shared<UDPInterfaceEndpoint>(endpoint->address(), endpoint->port());
		SocketEndpoint socket_endpoint(getParent().get(), interface_endpoint);
		auto new_connection = std::make_shared<UDPConnection>(socket_endpoint, shared_from_this());
		new_connection->receivedConnect();
	}

	receive();
}

void UDPInterface::handleSend(Socket::SendCallback callback,
		const boost::system::error_code& error, std::shared_ptr<Connection> connection, std::string message, int bytes){
	callback(error.value(), connection, message, bytes);
}

void UDPInterface::receive() {
	auto received_from = std::make_shared<udp::endpoint>(local);
	//char* buffer = new char[MAX_UDP_PACKET_SIZE];
	charbuf_ptr_t buffer = std::make_shared<charbuf_t>();

	m_socket.async_receive_from(boost::asio::buffer(buffer->data(), MAX_UDP_PACKET_SIZE), *received_from,
			boost::bind(&UDPInterface::handleReceive, this, boost::asio::placeholders::error, buffer,
					boost::asio::placeholders::bytes_transferred, received_from));
}

void UDPInterface::send(std::shared_ptr<Connection> connection, const std::string& data, Socket::SendCallback callback) {
	auto interface_endpoint = std::static_pointer_cast<IPInterfaceEndpoint>(connection->getEndpoint().getInterfaceEndpoint());
	udp::endpoint dest(interface_endpoint->getAsioIP(), interface_endpoint->getPort());

	m_socket.async_send_to(boost::asio::buffer(data), dest,
			boost::bind(&UDPInterface::handleSend, this, callback, boost::asio::placeholders::error, connection, data, boost::asio::placeholders::bytes_transferred));
}

std::shared_ptr<InterfaceEndpoint> UDPInterface::createEndpoint() {
	return std::make_shared<UDPInterfaceEndpoint>();
}

void UDPInterface::setReceiveCallback(Socket::ReceiveCallback callback){
	receive_callback = callback;

	receive();
}
void UDPInterface::setDisconnectCallback(Socket::DisconnectCallback callback){
	disconnect_callback = callback;
}

void UDPInterface::connect(SocketEndpoint endpoint, Socket::ConnectCallback callback){
	auto new_connection = std::make_shared<UDPConnection>(endpoint, shared_from_this());
	new_connection->connect([&, this](int error, std::shared_ptr<Connection> connection){
		this->zygote_connections.erase(std::static_pointer_cast<UDPConnection>(connection));
		callback(error, connection);
	});
}

} /* namespace net */
} /* namespace p2pnet */
