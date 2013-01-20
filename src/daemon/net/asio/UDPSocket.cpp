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
std::string UDPSocketDestination::toString(){
	UDPSocketDestination_s dest;
	dest.set_ip(this->getIP());
	dest.set_port(this->getPort());
	return dest.SerializeAsString();
}
void UDPSocketDestination::fromString(const std::string& from){
	UDPSocketDestination_s dest;
	dest.ParseFromString(from);
	this->setIP(dest.ip());
	this->setPort(dest.port());
}

// UDPSocket
packet_info_t UDPSocket::makePacket(std::string message,
		UDPSocketDestination from, MessageSocketListener* observer) {
	packet_info_t packet;
	packet.destination = from.toString();
	packet.message = message;
	packet.socket_ptr = this;
	return packet;
}

UDPSocket::UDPSocket() :
		m_io_service(AsioIOService::getIOService()), m_socket(m_io_service) {
}

UDPSocket::~UDPSocket() {
	close();
}

void UDPSocket::bind(UDPSocketDestination destination) {
	m_socket.open(destination.getEndpoint().protocol());
	m_socket.bind(destination.getEndpoint());
}

UDPSocketDestination UDPSocket::getBound(){
	UDPSocketDestination dest;
	dest.setEndpoint(m_socket.local_endpoint());
	return dest;
}

void UDPSocket::close() {
	if(m_socket.is_open())
		m_socket.close();
}

void UDPSocket::async_receive_from(UDPSocketDestination &dest, MessageSocketListener* observer) {
	if(!m_socket.is_open())
		m_socket.open(dest.getEndpoint().protocol());
	char data_received[MAX_PACKET_LENGTH];
	m_socket.async_receive_from(
			boost::asio::buffer(data_received, MAX_PACKET_LENGTH), dest.getEndpoint(),
			boost::bind(&UDPSocket::receive_handler, this, observer,
					data_received, boost::asio::placeholders::bytes_transferred, dest));
}

void UDPSocket::async_send_to(UDPSocketDestination &dest, std::string data, MessageSocketListener* observer) {
	if(!m_socket.is_open())
		m_socket.open(dest.getEndpoint().protocol());
	m_socket.async_send_to(
			boost::asio::buffer(data), dest.getEndpoint(),
			boost::bind(&UDPSocket::send_handler, this, observer, data, boost::asio::placeholders::bytes_transferred, dest));
}

void UDPSocket::wait_receive_from(UDPSocketDestination &dest, MessageSocketListener* observer) {
	if(!m_socket.is_open())
		m_socket.open(dest.getEndpoint().protocol());
	char data_received[MAX_PACKET_LENGTH];
	size_t bytes_received = m_socket.receive_from(boost::asio::buffer(data_received), dest.getEndpoint());

	std::string received_string();
	receive_handler(observer, data_received, bytes_received, dest);
}

void UDPSocket::wait_send_to(UDPSocketDestination &dest, std::string data, MessageSocketListener* observer) {
	if(!m_socket.is_open())
		m_socket.open(dest.getEndpoint().protocol());
	size_t bytes_sent = m_socket.send_to(boost::asio::buffer(data), dest.getEndpoint());
	send_handler(observer, data, bytes_sent, dest);
}

packet_info_t UDPSocket::here_receive_from(UDPSocketDestination &dest) {
	if(!m_socket.is_open())
		m_socket.open(dest.getEndpoint().protocol());
	char data_received[MAX_PACKET_LENGTH];
	size_t bytes_received = m_socket.receive_from(boost::asio::buffer(data_received, MAX_PACKET_LENGTH), dest.getEndpoint());

	packet_info_t packet_info;
	packet_info.socket_ptr = this;
	packet_info.message = std::string(data_received, bytes_received);

	return packet_info;
}

void UDPSocket::here_send_to(UDPSocketDestination &dest, std::string data) {
	if(!m_socket.is_open())
		m_socket.open(dest.getEndpoint().protocol());
	m_socket.send_to(boost::asio::buffer(data), dest.getEndpoint());
}


void UDPSocket::receive_handler(MessageSocketListener* observer, char* buffer, size_t bytes_received, UDPSocketDestination dest) {
	observer->receivedMessage(this->makePacket(std::string(buffer, bytes_received), dest, observer));
}

void UDPSocket::send_handler(MessageSocketListener* observer, std::string buffer, size_t bytes_sent, UDPSocketDestination dest) {
	observer->sentMessage(this->makePacket(buffer, dest, observer));
}

}
/* namespace net */
} /* namespace p2pnet */
