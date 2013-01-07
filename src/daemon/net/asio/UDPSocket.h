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

	std::string ip;
	unsigned short port;

	UDPSocketDestination(std::string ip, unsigned short port){
		this->ip = ip;
		this->port = port;
	}

	bool isIPv4() {
		return boost::asio::ip::address::from_string(ip).is_v4();
	}
	bool isIPv6() {
		return boost::asio::ip::address::from_string(ip).is_v6();
	}
	udp::endpoint toEndpoint() {
		return udp::endpoint(address::from_string(ip), port);
	}
	void fromEndpoint(udp::endpoint endpoint) {
		ip = endpoint.address().to_string();
		port = endpoint.port();
	}
};

class UDPSocket: public MessageSocket {
public:
	UDPSocket();
	virtual ~UDPSocket();

	void bind(UDPSocketDestination destination);
	void connect(UDPSocketDestination destination);

	void close();

	// Send/receive functions
	virtual void async_receive(DataObserver* observer);
	virtual void async_continious_receive(DataObserver* observer);
	virtual void async_send(std::string data, DataObserver* observer);

	virtual void wait_receive(DataObserver* observer);
	virtual void wait_send(std::string data, DataObserver* observer);

	virtual packet_info_t here_receive();
	virtual void here_send(std::string data);

private:
	void receive_handler(DataObserver* observer, char* buffer, size_t bytes_received);
	void continious_receive_handler(DataObserver* observer, char* buffer, size_t bytes_received);
	void send_handler(DataObserver* observer, size_t bytes_sent);

	boost::asio::io_service& m_io_service;
	udp::socket m_socket;

	udp::endpoint m_bind_endpoint;
	udp::endpoint m_connect_endpoint;
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* UDPSOCKET_H_ */
