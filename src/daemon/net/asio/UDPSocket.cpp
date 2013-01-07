/*
 * UDPSocket.cpp
 *
 *  Created on: 17.12.2012
 *      Author: gamepad
 */

#include "UDPSocket.h"
#include "AsioIOService.h"
#include <boost/bind.hpp>
#include <string>

using namespace boost::asio::ip;

namespace p2pnet {
namespace net {

UDPSocket::UDPSocket() :
		m_io_service(AsioIOService::getIOService()), m_socket(m_io_service) {
}

UDPSocket::~UDPSocket() {
	close();
}

void UDPSocket::bind(UDPSocketDestination destination) {
	m_bind_endpoint = destination.toEndpoint();
	m_socket.open(m_bind_endpoint.protocol());
	m_socket.bind(m_bind_endpoint);
}

void UDPSocket::connect(UDPSocketDestination destination) {
	m_connect_endpoint = destination.toEndpoint();
	m_socket.open(m_connect_endpoint.protocol());
	m_socket.connect(m_connect_endpoint);
}

void UDPSocket::close() {
	if(m_socket.is_open())
		m_socket.close();
}

void UDPSocket::async_receive(DataObserver* observer) {
	char data_received[MAX_PACKET_LENGTH];
	m_socket.async_receive(
			boost::asio::buffer(data_received, MAX_PACKET_LENGTH),
			boost::bind(&UDPSocket::receive_handler, this, observer,
					data_received, boost::asio::placeholders::bytes_transferred));
}

void UDPSocket::async_continious_receive(DataObserver* observer) {
	char data_received[MAX_PACKET_LENGTH];
	udp::endpoint endpoint;
	m_socket.async_receive_from(
			boost::asio::buffer(data_received, MAX_PACKET_LENGTH), endpoint,
			boost::bind(&UDPSocket::receive_handler, this, observer,
					data_received, boost::asio::placeholders::bytes_transferred));
}

void UDPSocket::async_send(std::string data, DataObserver* observer) {
	m_socket.async_send(
			boost::asio::buffer(data),
			boost::bind(&UDPSocket::send_handler, this, observer, boost::asio::placeholders::bytes_transferred));
}

void UDPSocket::wait_receive(DataObserver* observer) {
	char data_received[MAX_PACKET_LENGTH];
	size_t bytes_received = m_socket.receive_from(boost::asio::buffer(data_received), m_connect_endpoint);

	std::string received_string();
	receive_handler(observer, data_received, bytes_received);
}

void UDPSocket::wait_send(std::string data, DataObserver* observer) {
	size_t bytes_sent = m_socket.send_to(boost::asio::buffer(data), m_connect_endpoint);
	send_handler(observer, bytes_sent);
}

packet_info_t UDPSocket::here_receive() {
	char data_received[MAX_PACKET_LENGTH];
	size_t bytes_received = m_socket.receive_from(boost::asio::buffer(data_received, MAX_PACKET_LENGTH), m_connect_endpoint);

	packet_info_t packet_info;
	packet_info.socket_ptr = this;
	packet_info.message = std::string(data_received, bytes_received);

	return packet_info;
}

void UDPSocket::here_send(std::string data) {
	m_socket.send_to(boost::asio::buffer(data), m_connect_endpoint);
}

void UDPSocket::receive_handler(DataObserver* observer, char* buffer, size_t bytes_received) {
	packet_info_t message;
	message.message = std::string(buffer, bytes_received);
	message.socket_ptr = this;
	observer->receivedMessage(message);
}

void UDPSocket::send_handler(DataObserver* observer, size_t bytes_sent) {

}

}
/* namespace net */
} /* namespace p2pnet */
