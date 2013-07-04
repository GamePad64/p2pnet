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

#include "../TransportSocketListener.h"
#include "UDPTransportInterface.h"
#include "../../AsioIOService.h"
#include <boost/bind.hpp>
#include <string>
#include <memory>

namespace p2pnet {
namespace net {

UDPTransportInterface::UDPTransportInterface() :
		m_io_service(AsioIOService::getIOService()), m_socket(m_io_service) {
	mtu = IPv4_MTU < IPv6_MTU ? IPv4_MTU : IPv6_MTU;
}

UDPTransportInterface::~UDPTransportInterface() {
	if (m_socket.is_open())
		m_socket.close();
}

void UDPTransportInterface::receivedMessageHandler(const std::string& message,
		TransportInterfaceEndpoint::pointer endpoint_p) {
	TransportInterfaceCallback callback;
	callback.data = message;
	callback.endpoint = endpoint_p;
	this->updateOnReceive(callback);
}

void UDPTransportInterface::receivedMessageHandler(char* buffer,
		size_t bytes_received, TransportInterfaceEndpoint::pointer endpoint_p) {
	std::string message(buffer, bytes_received);
	delete[] buffer;
	this->receivedMessageHandler(message, endpoint_p);
}

void UDPTransportInterface::sentMessageHandler(const std::string& message,
		TransportInterfaceEndpoint::pointer endpoint_p) {
	TransportInterfaceCallback callback;
	callback.data = message;
	callback.endpoint = endpoint_p;
	this->updateOnSend(callback);
}

void UDPTransportInterface::sentMessageHandler(char* buffer, size_t bytes_sent,
		TransportInterfaceEndpoint::pointer endpoint_p) {
	std::string message(buffer, bytes_sent);
	delete[] buffer;
	this->sentMessageHandler(message, endpoint_p);
}

void UDPTransportInterface::openIPv4() {
	mtu = IPv4_MTU;
	m_socket.open(ip::udp::endpoint::protocol_type::v4());
}

void UDPTransportInterface::openIPv6() {
	mtu = IPv6_MTU;
	m_socket.open(ip::udp::endpoint::protocol_type::v6());
}

void UDPTransportInterface::openAll() {
	mtu = IPv4_MTU;
	m_socket.open(ip::udp::endpoint::protocol_type::v6());
}

void UDPTransportInterface::bindLocalIPv4(
		UDPTransportInterfaceEndpoint::port_t port) {
	ip::udp::endpoint local_ipv4(ip::address_v4::from_string("0.0.0.0"), port);
	openIPv4();
	m_socket.bind(local_ipv4);
}

void UDPTransportInterface::bindLocalIPv6(
		UDPTransportInterfaceEndpoint::port_t port) {
	ip::udp::endpoint local_ipv6(ip::address_v6::from_string("0::0"), port);
	openIPv6();
	m_socket.set_option(boost::asio::ip::v6_only(true));
	m_socket.bind(local_ipv6);
}

void UDPTransportInterface::bindLocalAll(
		UDPTransportInterfaceEndpoint::port_t port) {
	ip::udp::endpoint local_ipv6(ip::address_v6::from_string("0::0"), port);
	openAll();
	m_socket.bind(local_ipv6);
}

//Inherited from TransportSocket
void UDPTransportInterface::asyncReceiveFrom(TransportInterfaceEndpoint::const_pointer endpoint) {
	std::shared_ptr<UDPTransportInterfaceEndpoint> mutable_copy_endpoint = std::make_shared<UDPTransportInterfaceEndpoint>();
	std::shared_ptr<const UDPTransportInterfaceEndpoint> const_copy_endpoint = std::dynamic_pointer_cast<const UDPTransportInterfaceEndpoint>(endpoint);
	*mutable_copy_endpoint = *const_copy_endpoint;

	char* data_received = new char[getMTU()];
	m_socket.async_receive_from(
			boost::asio::buffer(data_received, getMTU()),
			mutable_copy_endpoint->getEndpoint(),
			boost::bind(&UDPTransportInterface::receivedMessageHandler, this, data_received,
					boost::asio::placeholders::bytes_transferred, mutable_copy_endpoint));
}

void UDPTransportInterface::asyncSendTo(TransportInterfaceEndpoint::const_pointer endpoint,	const std::string& data) {
	std::shared_ptr<UDPTransportInterfaceEndpoint> mutable_copy_endpoint = std::make_shared<UDPTransportInterfaceEndpoint>();
	std::shared_ptr<const UDPTransportInterfaceEndpoint> const_copy_endpoint = std::dynamic_pointer_cast<const UDPTransportInterfaceEndpoint>(endpoint);
	*mutable_copy_endpoint = *const_copy_endpoint;

	char* data_sent = new char[getMTU()];
	m_socket.async_send_to(boost::asio::buffer(data), mutable_copy_endpoint->getEndpoint(),
			boost::bind(&UDPTransportInterface::sentMessageHandler, this, data_sent, boost::asio::placeholders::bytes_transferred, mutable_copy_endpoint));
}

void UDPTransportInterface::waitReceiveFrom(TransportInterfaceEndpoint::const_pointer endpoint) {
	std::shared_ptr<UDPTransportInterfaceEndpoint> mutable_copy_endpoint = std::make_shared<UDPTransportInterfaceEndpoint>();
	std::shared_ptr<const UDPTransportInterfaceEndpoint> const_copy_endpoint = std::dynamic_pointer_cast<const UDPTransportInterfaceEndpoint>(endpoint);
	*mutable_copy_endpoint = *const_copy_endpoint;

	char* data_received = new char[getMTU()];
	size_t bytes_received = m_socket.receive_from(
			boost::asio::buffer(data_received, getMTU()),
			mutable_copy_endpoint->getEndpoint());

	receivedMessageHandler(data_received, bytes_received, mutable_copy_endpoint);
}

void UDPTransportInterface::waitSendTo(TransportInterfaceEndpoint::const_pointer endpoint, const std::string& data) {
	std::shared_ptr<UDPTransportInterfaceEndpoint> mutable_copy_endpoint = std::make_shared<UDPTransportInterfaceEndpoint>();
	std::shared_ptr<const UDPTransportInterfaceEndpoint> const_copy_endpoint = std::dynamic_pointer_cast<const UDPTransportInterfaceEndpoint>(endpoint);
	*mutable_copy_endpoint = *const_copy_endpoint;

	/*size_t bytes_sent = */m_socket.send_to(boost::asio::buffer(data), mutable_copy_endpoint->getEndpoint());

	sentMessageHandler(data, mutable_copy_endpoint);
}

TransportInterfaceCallback UDPTransportInterface::hereReceiveFrom(TransportInterfaceEndpoint::const_pointer endpoint) {
	std::shared_ptr<UDPTransportInterfaceEndpoint> mutable_copy_endpoint = std::make_shared<UDPTransportInterfaceEndpoint>();
	std::shared_ptr<const UDPTransportInterfaceEndpoint> const_copy_endpoint = std::dynamic_pointer_cast<const UDPTransportInterfaceEndpoint>(endpoint);
	*mutable_copy_endpoint = *const_copy_endpoint;

	char* data_received = new char[getMTU()];
	size_t bytes_received = m_socket.receive_from(boost::asio::buffer(data_received, getMTU()), mutable_copy_endpoint->getEndpoint());

	TransportInterfaceCallback callback;
	callback.data = std::string(data_received, bytes_received);
	callback.endpoint = mutable_copy_endpoint;

	delete[] data_received;
	return callback;
}

void UDPTransportInterface::hereSendTo(TransportInterfaceEndpoint::const_pointer endpoint, const std::string& data) {
	std::shared_ptr<UDPTransportInterfaceEndpoint> mutable_copy_endpoint = std::make_shared<UDPTransportInterfaceEndpoint>();
	std::shared_ptr<const UDPTransportInterfaceEndpoint> const_copy_endpoint = std::dynamic_pointer_cast<const UDPTransportInterfaceEndpoint>(endpoint);
	*mutable_copy_endpoint = *const_copy_endpoint;

	m_socket.send_to(boost::asio::buffer(data), mutable_copy_endpoint->getEndpoint());
}

} /* namespace net */
} /* namespace p2pnet */
