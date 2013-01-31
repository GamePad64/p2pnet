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
		asio_io_service(AsioIOService::getIOService()), asio_socket(
				asio_io_service) {

}

UDPTransportSocket::~UDPTransportSocket() {
	if (asio_socket.is_open())
		asio_socket.close();
}

void UDPTransportSocket::openIPv4() {
	asio_socket.open(ip::udp::endpoint::protocol_type::v4());
	max_packet_length = IPv4_MTU;
}

void UDPTransportSocket::openIPv6() {
	asio_socket.open(ip::udp::endpoint::protocol_type::v6());
	max_packet_length = IPv6_MTU;
}

void UDPTransportSocket::receivedMessageHandler(
		TransportSocketListener* listener, char* buffer, size_t bytes_received,
		UDPTransportSocketEndpoint* endpoint,
		TransportSocketConnection connection) {
	listener->receivedMessage(createMessageBundle(std::string(buffer, bytes_received), *endpoint, connection));
}

void UDPTransportSocket::sentMessageHandler(TransportSocketListener* listener,
		char* buffer, size_t bytes_sent,
		UDPTransportSocketEndpoint* endpoint,
		TransportSocketConnection connection) {
	listener->sentMessage(createMessageBundle(std::string(buffer, bytes_sent), *endpoint, connection));
}

void UDPTransportSocket::bindLocalIPv4(
		UDPTransportSocketEndpoint::port_t port) {
	ip::udp::endpoint local_ipv4(ip::address_v4::from_string("0.0.0.0"), port);
	asio_socket.open(local_ipv4.protocol());
	asio_socket.bind(local_ipv4);

	max_packet_length = IPv4_MTU;
}

void UDPTransportSocket::bindLocalIPv6(
		UDPTransportSocketEndpoint::port_t port) {
	ip::udp::endpoint local_ipv6(ip::address_v4::from_string("0::0"), port);
	asio_socket.open(local_ipv6.protocol());
	asio_socket.bind(local_ipv6);

	max_packet_length = IPv6_MTU;
}

//Inherited from TransportSocket
void UDPTransportSocket::asyncReceiveFrom(TransportSocketEndpoint& endpoint,
		TransportSocketListener* listener,
		TransportSocketConnection connection) {
	char* data_received = new char[getMaxPacketLength()];
	asio_socket.async_receive_from(
			boost::asio::buffer(data_received, getMaxPacketLength()),
			dynamic_cast<UDPTransportSocketEndpoint&>(endpoint).getEndpoint(),
			boost::bind(&UDPTransportSocket::receivedMessageHandler, this,
					listener, data_received,
					boost::asio::placeholders::bytes_transferred,
					dynamic_cast<UDPTransportSocketEndpoint*>(&endpoint),
					connection));
}

void UDPTransportSocket::asyncSendTo(TransportSocketEndpoint& endpoint,
		const std::string data, TransportSocketListener* listener,
		TransportSocketConnection connection) {
	asio_socket.async_send_to(boost::asio::buffer(data),
			dynamic_cast<UDPTransportSocketEndpoint&>(endpoint).getEndpoint(),
			boost::bind(&UDPTransportSocket::sentMessageHandler, this, listener,
					const_cast<char*>(data.data()), boost::asio::placeholders::bytes_transferred,
					dynamic_cast<UDPTransportSocketEndpoint*>(&endpoint),
					connection));
}

void UDPTransportSocket::waitReceiveFrom(TransportSocketEndpoint& endpoint,
		TransportSocketListener* listener,
		TransportSocketConnection connection) {
	char* data_received = new char[getMaxPacketLength()];
	size_t bytes_received = asio_socket.receive_from(
			boost::asio::buffer(data_received, getMaxPacketLength()),
			dynamic_cast<UDPTransportSocketEndpoint&>(endpoint).getEndpoint());

	receivedMessageHandler(listener, data_received, bytes_received,
			dynamic_cast<UDPTransportSocketEndpoint*>(&endpoint), connection);
}

void UDPTransportSocket::waitSendTo(TransportSocketEndpoint& endpoint,
		const std::string data, TransportSocketListener* listener,
		TransportSocketConnection connection) {
	size_t bytes_sent = asio_socket.send_to(boost::asio::buffer(data),
			dynamic_cast<UDPTransportSocketEndpoint&>(endpoint).getEndpoint());

	sentMessageHandler(listener, const_cast<char*>(data.data()), bytes_sent,
			dynamic_cast<UDPTransportSocketEndpoint*>(&endpoint), connection);
}

MessageBundle UDPTransportSocket::hereReceiveFrom(
		TransportSocketEndpoint& endpoint,
		TransportSocketConnection connection) {
	char* data_received = new char[getMaxPacketLength()];

	size_t bytes_received = asio_socket.receive_from(boost::asio::buffer(data_received, getMaxPacketLength()), dynamic_cast<UDPTransportSocketEndpoint&>(endpoint).getEndpoint());
	MessageBundle bundle = this->createMessageBundle(std::string(data_received, bytes_received), endpoint, connection);
	delete[] data_received;
	return bundle;
}

void UDPTransportSocket::hereSendTo(TransportSocketEndpoint& endpoint,
		const std::string data) {
	asio_socket.send_to(boost::asio::buffer(data), dynamic_cast<UDPTransportSocketEndpoint&>(endpoint).getEndpoint());
}

} /* namespace net */
} /* namespace p2pnet */
