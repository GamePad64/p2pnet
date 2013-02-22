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

#include "TransportSocketListener.h"
#include "UDPTransportSocket.h"
#include "../AsioIOService.h"
#include <boost/bind.hpp>
#include <string>

namespace p2pnet {
namespace net {

UDPTransportSocket::UDPTransportSocket() :
		m_io_service(AsioIOService::getIOService()), m_socket(m_io_service) {}

UDPTransportSocket::~UDPTransportSocket() {
	if (m_socket.is_open())
		m_socket.close();
}

void UDPTransportSocket::receivedMessageHandler(const std::string& message,
		TransportSocketEndpoint::pointer endpoint_p) {
	this->updateOnReceive(this->createMessageBundle(message, endpoint_p));
}

void UDPTransportSocket::receivedMessageHandler(char* buffer,
		size_t bytes_received, TransportSocketEndpoint::pointer endpoint_p) {
	std::string message(buffer, bytes_received);
	delete[] buffer;
	this->receivedMessageHandler(message, endpoint_p);
}

void UDPTransportSocket::sentMessageHandler(const std::string& message,
		TransportSocketEndpoint::pointer endpoint_p) {
	this->updateOnSend(this->createMessageBundle(message, endpoint_p));
}

void UDPTransportSocket::sentMessageHandler(char* buffer, size_t bytes_sent,
		TransportSocketEndpoint::pointer endpoint_p) {
	std::string message(buffer, bytes_sent);
	delete[] buffer;
	this->updateOnReceive(this->createMessageBundle(message, endpoint_p));
}

void UDPTransportSocket::openIPv4() {
	m_socket.open(ip::udp::endpoint::protocol_type::v4());
	max_packet_length = IPv4_MTU;
}

void UDPTransportSocket::openIPv6() {
	m_socket.open(ip::udp::endpoint::protocol_type::v6());
	max_packet_length = IPv6_MTU;
}

void UDPTransportSocket::bindLocalIPv4(
		UDPTransportSocketEndpoint::port_t port) {
	ip::udp::endpoint local_ipv4(ip::address_v4::from_string("0.0.0.0"), port);
	m_socket.open(local_ipv4.protocol());
	m_socket.bind(local_ipv4);

	max_packet_length = IPv4_MTU;
}

void UDPTransportSocket::bindLocalIPv6(
		UDPTransportSocketEndpoint::port_t port) {
	ip::udp::endpoint local_ipv6(ip::address_v6::from_string("0::0"), port);
	m_socket.open(local_ipv6.protocol());
	m_socket.set_option(boost::asio::ip::v6_only(true));
	m_socket.bind(local_ipv6);

	max_packet_length = IPv6_MTU;
}

//Inherited from TransportSocket
void UDPTransportSocket::asyncReceiveFrom(TransportSocketEndpoint::pointer endpoint) {
	UDPTransportSocketEndpoint::udp_pointer udp_endpoint = std::static_pointer_cast<UDPTransportSocketEndpoint>(endpoint);

	char* data_received = new char[getMaxPacketLength()];
	m_socket.async_receive_from(
			boost::asio::buffer(data_received, getMaxPacketLength()),
			udp_endpoint->getEndpoint(),
			boost::bind(&UDPTransportSocket::receivedMessageHandler, this, data_received,
					boost::asio::placeholders::bytes_transferred, udp_endpoint));
}

void UDPTransportSocket::asyncSendTo(TransportSocketEndpoint::pointer endpoint,	const std::string& data) {
	UDPTransportSocketEndpoint::udp_pointer udp_endpoint = std::static_pointer_cast<UDPTransportSocketEndpoint>(endpoint);

	char* data_sent = new char[getMaxPacketLength()];
	m_socket.async_send_to(boost::asio::buffer(data), udp_endpoint->getEndpoint(),
			boost::bind(&UDPTransportSocket::sentMessageHandler, this, data_sent, boost::asio::placeholders::bytes_transferred, udp_endpoint));
}

void UDPTransportSocket::waitReceiveFrom(TransportSocketEndpoint::pointer endpoint) {
	UDPTransportSocketEndpoint::udp_pointer udp_endpoint = std::static_pointer_cast<UDPTransportSocketEndpoint>(endpoint);

	char* data_received = new char[getMaxPacketLength()];
	size_t bytes_received = m_socket.receive_from(
			boost::asio::buffer(data_received, getMaxPacketLength()),
			udp_endpoint->getEndpoint());

	receivedMessageHandler(data_received, bytes_received, udp_endpoint);
}

void UDPTransportSocket::waitSendTo(TransportSocketEndpoint::pointer endpoint, const std::string& data) {
	UDPTransportSocketEndpoint::udp_pointer udp_endpoint = std::static_pointer_cast<UDPTransportSocketEndpoint>(endpoint);

	/*size_t bytes_sent = */m_socket.send_to(boost::asio::buffer(data), udp_endpoint->getEndpoint());

	sentMessageHandler(data, udp_endpoint);
}

MessageBundle UDPTransportSocket::hereReceiveFrom(TransportSocketEndpoint::pointer endpoint) {
	UDPTransportSocketEndpoint::udp_pointer udp_endpoint = std::static_pointer_cast<UDPTransportSocketEndpoint>(endpoint);

	char* data_received = new char[getMaxPacketLength()];
	size_t bytes_received = m_socket.receive_from(boost::asio::buffer(data_received, getMaxPacketLength()), udp_endpoint->getEndpoint());

	MessageBundle bundle = this->createMessageBundle(std::string(data_received, bytes_received), udp_endpoint);

	delete[] data_received;
	return bundle;
}

void UDPTransportSocket::hereSendTo(TransportSocketEndpoint::pointer endpoint, const std::string& data) {
	UDPTransportSocketEndpoint::udp_pointer udp_endpoint = std::static_pointer_cast<UDPTransportSocketEndpoint>(endpoint);
	m_socket.send_to(boost::asio::buffer(data), udp_endpoint->getEndpoint());
}

} /* namespace net */
} /* namespace p2pnet */
