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
	udp::endpoint asio_endpoint;
public:
	typedef unsigned short int port_t;

	UDPSocketDestination(){};

	udp::endpoint& getEndpoint(){return this->asio_endpoint;};
	void setEndpoint(const udp::endpoint& endpoint){this->asio_endpoint = endpoint;};

	bool isIPv4(){return this->asio_endpoint.address().is_v4();};
	bool isIPv6(){return this->asio_endpoint.address().is_v6();};

	std::string getIP(){return this->asio_endpoint.address().to_string();};
	void setIP(const std::string& ip){this->asio_endpoint.address(address::from_string(ip));};
	port_t getPort(){return this->asio_endpoint.port();};
	void setPort(port_t port){this->asio_endpoint.port(port);};
	UDPSocketDestination(std::string ip, port_t port){this->setIP(ip);this->setPort(port);};

	std::string toString();	//! Serialize to string using protobuf
	void fromString(const std::string& from);	//! Parse from string using protobuf

	UDPSocketDestination(const std::string& from){this->fromString(from);};	//! Constructor, which constructs endpoint from protobuf'ed string
};

class UDPSocket {
	packet_info_t makePacket(std::string message, UDPSocketDestination dest, MessageSocketListener* observer);

public:
	UDPSocket();
	virtual ~UDPSocket();

	void bind(UDPSocketDestination destination);
	UDPSocketDestination getBound();

	void close();

	// Send/receive functions
	virtual void async_receive_from(UDPSocketDestination &dest, MessageSocketListener* observer);
	virtual void async_send_to(UDPSocketDestination &dest, std::string data, MessageSocketListener* observer);

	virtual void wait_receive_from(UDPSocketDestination &dest, MessageSocketListener* observer);
	virtual void wait_send_to(UDPSocketDestination &dest, std::string data, MessageSocketListener* observer);

	virtual packet_info_t here_receive_from(UDPSocketDestination &dest);
	virtual void here_send_to(UDPSocketDestination &dest, std::string data);

private:
	void receive_handler(MessageSocketListener* observer, char* buffer, size_t bytes_received, UDPSocketDestination dest);
	void send_handler(MessageSocketListener* observer, std::string buffer, size_t bytes_sent, UDPSocketDestination dest);

	boost::asio::io_service& m_io_service;
	udp::socket m_socket;
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* UDPSOCKET_H_ */
