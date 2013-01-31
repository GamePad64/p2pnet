/*
 * UDPTransportSocketEndpoint.h
 *
 *  Created on: 27.01.2013
 *      Author: gamepad
 */

#ifndef UDPTRANSPORTSOCKETENDPOINT_H_
#define UDPTRANSPORTSOCKETENDPOINT_H_

#include "TransportSocketEndpoint.h"
#include <boost/asio.hpp>

namespace p2pnet {
namespace net {

using namespace boost::asio::ip;

class UDPTransportSocketEndpoint: public p2pnet::net::TransportSocketEndpoint {
	friend class UDPTransportSocket;
private:
	udp::endpoint& getEndpoint();
	void setEndpoint(const udp::endpoint& endpoint);
protected:
	udp::endpoint asio_endpoint;
public:
	UDPTransportSocketEndpoint();
	virtual ~UDPTransportSocketEndpoint();

	/*!
	 * This function returns true if IP address is an IPv4 address.
	 * @return bool
	 */
	bool isIPv4();
	/*!
	 * This function returns true if IP address is an IPv6 address.
	 * @return bool
	 */
	bool isIPv6();

	/*!
	 * This function returns IP address of the endpoint.
	 * @return std::string in "255.255.255.255" format (if IPv4) or in hexadecimal IPv6 representation.
	 */
	std::string getIP();
	/*!
	 * This function sets IP address of the endpoint
	 * @param ip IP address in IPv4 or IPv6 format.
	 */
	void setIP(const std::string& ip);

	typedef unsigned short int port_t;
	port_t getPort();
	void setPort(port_t port);
	/*!
	 * This is a constructor of UDPTransportSocketEndpoint with setting IP and port.
	 * @param ip IP address in IPv4 or IPv6 format.
	 * @param port Port ranging from 1 to 65535.
	 */
	UDPTransportSocketEndpoint(std::string ip, port_t port);

	virtual std::string toString();
	virtual void fromString(std::string endpoint_s);
	UDPTransportSocketEndpoint(std::string endpoint_s);
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* UDPTRANSPORTSOCKETENDPOINT_H_ */
