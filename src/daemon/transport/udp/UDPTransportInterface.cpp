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

#include "UDPTransportInterface.h"
#include "../../AsioIOService.h"
#include <boost/bind.hpp>
#include <string>
#include <memory>

namespace p2pnet {
namespace transport {

UDPTransportInterface::UDPTransportInterface() :
		m_io_service(AsioIOService::getIOService()), m_socket(m_io_service) {
	mtu = IPv4_MTU < IPv6_MTU ? IPv4_MTU : IPv6_MTU;
}

UDPTransportInterface::~UDPTransportInterface() {
	if (m_socket.is_open())
		m_socket.close();
}

std::shared_ptr<TransportInterfaceEndpoint> UDPTransportInterface::createEndpoint() {
	return std::make_shared<UDPTransportInterfaceEndpoint>();
}

std::string UDPTransportInterface::getInterfacePrefix() const {
	return "UDP";
}

void UDPTransportInterface::sentMessageHandler(std::string data, udp::endpoint* endpoint) {
	delete endpoint;
}

void UDPTransportInterface::receivedMessageHandler(boost::asio::streambuf* buffer, size_t bytes_received, udp::endpoint* endpoint) {
	std::string message(buffer->data(), bytes_received);
	delete buffer;

	TransportSocketEndpoint received_endpoint(std::make_shared<UDPTransportInterfaceEndpoint>(*endpoint));
	delete endpoint;

	auto connection_it = TransportSocket::getInstance()->m_connections.find(received_endpoint);

	if(connection_it == TransportSocket::getInstance()->m_connections.end()){
		std::shared_ptr<UDPTransportConnection> new_connection = std::make_shared<UDPTransportConnection>(received_endpoint, this);
		TransportSocket::getInstance()->m_connections.insert(std::make_pair(received_endpoint, new_connection));
		new_connection->process(message);
	}else{
		connection_it->second->process(message);
	}
}

void UDPTransportInterface::openIPv4() {
	mtu = IPv4_MTU;
	m_socket.open(udp::endpoint::protocol_type::v4());
}

void UDPTransportInterface::openIPv6() {
	mtu = IPv6_MTU;
	m_socket.open(udp::endpoint::protocol_type::v6());
}

void UDPTransportInterface::openAll() {
	mtu = IPv4_MTU;
	m_socket.open(udp::endpoint::protocol_type::v6());
}

void UDPTransportInterface::bindLocalIPv4(
		UDPTransportInterfaceEndpoint::port_t port) {
	local.address(address_v4::from_string("0.0.0.0"));
	local.port(port);
	openIPv4();
	m_socket.bind(local);
}

void UDPTransportInterface::bindLocalIPv6(
		UDPTransportInterfaceEndpoint::port_t port) {
	local.address(address_v6::from_string("0::0"));
	local.port(port);
	openIPv6();
	m_socket.set_option(boost::asio::ip::v6_only(true));
	m_socket.bind(local);
}

void UDPTransportInterface::bindLocalAll(
		UDPTransportInterfaceEndpoint::port_t port) {
	local.address(address_v6::from_string("0::0"));
	local.port(port);
	openAll();
	m_socket.bind(local);
}

//Inherited from TransportSocket
void UDPTransportInterface::receive() {
	udp::endpoint *received = udp::endpoint(local);

	boost::asio::streambuf* data_received = new boost::asio::streambuf();
	m_socket.async_receive_from(*data_received, *received,
			boost::bind(&UDPTransportInterface::receivedMessageHandler, this, data_received,
					boost::asio::placeholders::bytes_transferred, received));
}

void UDPTransportInterface::send(TransportSocketEndpoint dest, const std::string& data) {
	auto socketendpoint_ptr = new TransportSocketEndpoint(dest);
	// Creating new asio endpoint pointer from TransportSocketEndpoint below. "Braindanger ahead!"
	udp::endpoint* asiosocketendpoint_ptr = new udp::endpoint(static_cast<UDPTransportInterfaceEndpoint>(socketendpoint_ptr->getInterfaceEndpoint()).getEndpoint());

	m_socket.async_send_to(boost::asio::buffer(data), *asiosocketendpoint_ptr,
			boost::bind(&UDPTransportInterface::sentMessageHandler, this, data, asiosocketendpoint_ptr));
}

} /* namespace net */
} /* namespace p2pnet */
