/*
 * UDPTransportSocketEndpoint.cpp
 *
 *  Created on: 27.01.2013
 *      Author: gamepad
 */

#include "UDPTransportSocketEndpoint.h"
#include "../protobuf/TransportSocketEndpoint_s.pb.h"

namespace p2pnet {
namespace net {

UDPTransportSocketEndpoint::UDPTransportSocketEndpoint(){};
UDPTransportSocketEndpoint::~UDPTransportSocketEndpoint(){};

udp::endpoint& UDPTransportSocketEndpoint::getEndpoint(){
	return this->asio_endpoint;
};

void UDPTransportSocketEndpoint::setEndpoint(const udp::endpoint& endpoint){
	this->asio_endpoint = endpoint;
};

bool UDPTransportSocketEndpoint::isIPv4(){
	return this->asio_endpoint.address().is_v4();
};

bool UDPTransportSocketEndpoint::isIPv6(){
	return this->asio_endpoint.address().is_v6();
};

std::string UDPTransportSocketEndpoint::getIP(){
	return this->asio_endpoint.address().to_string();
};
void UDPTransportSocketEndpoint::setIP(const std::string& ip){
	this->asio_endpoint.address(address::from_string(ip));
};

UDPTransportSocketEndpoint::port_t UDPTransportSocketEndpoint::getPort(){
	return this->asio_endpoint.port();
};
void UDPTransportSocketEndpoint::setPort(UDPTransportSocketEndpoint::port_t port){
	this->asio_endpoint.port(port);
};
UDPTransportSocketEndpoint::UDPTransportSocketEndpoint(std::string ip, UDPTransportSocketEndpoint::port_t port){
	this->setIP(ip);
	this->setPort(port);
};

std::string UDPTransportSocketEndpoint::toString() {
	UDPTransportSocketEndpoint_s endpoint_pb;
	endpoint_pb.set_ip(this->getIP());
	endpoint_pb.set_port(this->getPort());
	return endpoint_pb.SerializeAsString();
}

void UDPTransportSocketEndpoint::fromString(std::string endpoint_s) {
	UDPTransportSocketEndpoint_s endpoint_pb;
	endpoint_pb.ParseFromString(endpoint_s);
	this->setIP(endpoint_pb.ip());
	this->setPort(endpoint_pb.port());
}

} /* namespace net */
} /* namespace p2pnet */
