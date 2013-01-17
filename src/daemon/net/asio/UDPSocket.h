/*
 * UDPSocket.h
 *
 *  Created on: 17.12.2012
 *      Author: gamepad
 */

#ifndef UDPSOCKET_H_
#define UDPSOCKET_H_

#include "../MessageSocket.h"
#include <boost/asio.hpp>
#include <string>

using namespace boost::asio::ip;

namespace p2pnet {
namespace net {

const std::string LOCAL_V4 = "0.0.0.0";
const std::string LOCAL_V6 = "0::0";

class UDPSocketDestination {
public:
	UDPSocketDestination();
	UDPSocketDestination(std::string ip, unsigned short port);

	std::string ip;
	unsigned short port;

	bool isIPv4();
	bool isIPv6();

	// Asio Endpoint conversion
	udp::endpoint toEndpoint();
	void fromEndpoint(udp::endpoint endpoint);

	// Protocol Buffers Serialization
	std::string toString();
	void fromString(const std::string& from);

	UDPSocketDestination(const std::string& from);
};

class UDPSocket: public net::MessageSocket {
public:
	UDPSocket();
	virtual ~UDPSocket();

	void bind(UDPSocketDestination destination);
	void connect(UDPSocketDestination destination);

	void close();

	// Send/receive functions
	virtual void async_receive(MessageSocketListener* observer);
	virtual void async_send(std::string data, MessageSocketListener* observer);

	virtual void wait_receive(MessageSocketListener* observer);
	virtual void wait_send(std::string data, MessageSocketListener* observer);

	virtual packet_info_t here_receive();
	virtual void here_send(std::string data);

private:
	void receive_handler(MessageSocketListener* observer, char* buffer, size_t bytes_received);
	void send_handler(MessageSocketListener* observer, size_t bytes_sent);

	boost::asio::io_service& m_io_service;
	udp::socket m_socket;

	udp::endpoint m_bind_endpoint;
	udp::endpoint m_connect_endpoint;
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* UDPSOCKET_H_ */
