/*
 * UDPSocket.cpp
 *
 *  Created on: 17.12.2012
 *      Author: gamepad
 */

#include "UDPSocket.h"
#include "AsioIOService.h"
#include "../../protobuf/net_dest.pb.h"
#include <boost/bind.hpp>
#include <string>

using namespace boost::asio::ip;

namespace p2pnet {
namespace net {

// UDPSocketDestination

UDPSocketDestination::UDPSocketDestination(std::string ip, unsigned short port){
	this->ip = ip;
	this->port = port;
}

bool UDPSocketDestination::isIPv4() {
	return boost::asio::ip::address::from_string(ip).is_v4();
}
bool UDPSocketDestination::isIPv6() {
	return boost::asio::ip::address::from_string(ip).is_v6();
}
udp::endpoint UDPSocketDestination::toEndpoint() {
	return udp::endpoint(address::from_string(ip), port);
}
void UDPSocketDestination::fromEndpoint(udp::endpoint endpoint) {
	ip = endpoint.address().to_string();
	port = endpoint.port();
}
std::string UDPSocketDestination::toString(){
	UDPSocketDestination_s dest;
	dest.set_ip(ip);
	dest.set_port(port);
	return dest.SerializeAsString();
}
void UDPSocketDestination::fromString(const std::string& from){
	UDPSocketDestination_s dest;
	dest.ParseFromString(from);
	ip = dest.ip();
	port = dest.port();
}

UDPSocketDestination::UDPSocketDestination(const std::string& from){
	ip = LOCAL_V4;
	port = 0;
	this->fromString(from);
}

// UDPSocket
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

void UDPSocket::async_receive(MessageSocketListener* observer) {
	char data_received[MAX_PACKET_LENGTH];
	m_socket.async_receive(
			boost::asio::buffer(data_received, MAX_PACKET_LENGTH),
			boost::bind(&UDPSocket::receive_handler, this, observer,
					data_received, boost::asio::placeholders::bytes_transferred));
}

void UDPSocket::async_continious_receive(MessageSocketListener* observer) {
	char data_received[MAX_PACKET_LENGTH];
	udp::endpoint endpoint;
	m_socket.async_receive_from(
			boost::asio::buffer(data_received, MAX_PACKET_LENGTH), endpoint,
			boost::bind(&UDPSocket::receive_handler, this, observer,
					data_received, boost::asio::placeholders::bytes_transferred));
}

void UDPSocket::async_send(std::string data, MessageSocketListener* observer) {
	m_socket.async_send(
			boost::asio::buffer(data),
			boost::bind(&UDPSocket::send_handler, this, observer, boost::asio::placeholders::bytes_transferred));
}

void UDPSocket::wait_receive(MessageSocketListener* observer) {
	char data_received[MAX_PACKET_LENGTH];
	size_t bytes_received = m_socket.receive_from(boost::asio::buffer(data_received), m_connect_endpoint);

	std::string received_string();
	receive_handler(observer, data_received, bytes_received);
}

void UDPSocket::wait_send(std::string data, MessageSocketListener* observer) {
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

void UDPSocket::receive_handler(MessageSocketListener* observer, char* buffer, size_t bytes_received) {
	packet_info_t message;
	message.message = std::string(buffer, bytes_received);
	message.socket_ptr = this;
	observer->receivedMessage(message);
}

void UDPSocket::send_handler(MessageSocketListener* observer, size_t bytes_sent) {
	//TODO: write this send_handler for UDPSocket
}

}
/* namespace net */
} /* namespace p2pnet */
