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

UDPTransportInterface::UDPTransportInterface(ConfigManager& config) : TransportInterface(config), m_socket(AsioIOService::getIOService()) {
	readConfig();
	mtu = IPv4_MTU < IPv6_MTU ? IPv4_MTU : IPv6_MTU;

	m_socket.open(udp::endpoint::protocol_type::v6());
	m_socket.bind(local);
}

UDPTransportInterface::~UDPTransportInterface() {
	if (m_socket.is_open())
		m_socket.close();
}

void UDPTransportInterface::readConfig() {
	local.address(address::from_string(getValue<std::string>("transport.udp.local_ip")));
	local.port(getValue<unsigned short>("transport.udp.port"));
}

void UDPTransportInterface::sentMessageHandler(std::string data, std::shared_ptr<udp::endpoint> endpoint) {}
void UDPTransportInterface::receivedMessageHandler(std::shared_ptr<boost::asio::streambuf> buffer,
		size_t bytes_received,
		std::shared_ptr<udp::endpoint> endpoint) {

	std::string message(buffer->data(), bytes_received);

	TransportSocketEndpoint socket_endpoint(endpoint);
	auto connection_it = TransportSocket::getInstance()->m_connections.find(socket_endpoint);

	if(connection_it == TransportSocket::getInstance()->m_connections.end()){
		auto new_connection = std::make_shared<UDPTransportConnection>(socket_endpoint, this);
		TransportSocket::getInstance()->m_connections.insert(std::make_pair(socket_endpoint, new_connection));
		new_connection->process(message);
	}else{
		connection_it->second->process(message);
	}
}

std::shared_ptr<TransportInterfaceEndpoint> UDPTransportInterface::createEndpoint() {
	return std::make_shared<UDPTransportInterfaceEndpoint>();
}

std::string UDPTransportInterface::getInterfacePrefix() const {
	return "UDP";
}

//Inherited from TransportSocket
void UDPTransportInterface::receive() {
	auto received_from = std::make_shared<udp::endpoint>(local);

	auto data_received = std::make_shared<boost::asio::streambuf>();
	m_socket.async_receive_from(*data_received, *received_from,
			boost::bind(&UDPTransportInterface::receivedMessageHandler, this, data_received,
					boost::asio::placeholders::bytes_transferred, received_from));
}

void UDPTransportInterface::send(TransportSocketEndpoint dest, const std::string& data) {
	auto socketendpoint_ptr = new TransportSocketEndpoint(dest);
	// Creating new asio endpoint pointer from TransportSocketEndpoint below. "Braindanger ahead!"
	auto asiosocketendpoint_ptr = std::make_shared<udp::endpoint>(static_cast<UDPTransportInterfaceEndpoint>(socketendpoint_ptr->getInterfaceEndpoint()).getEndpoint());

	m_socket.async_send_to(boost::asio::buffer(data), *asiosocketendpoint_ptr,
			boost::bind(&UDPTransportInterface::sentMessageHandler, this, data, asiosocketendpoint_ptr));
}

} /* namespace net */
} /* namespace p2pnet */