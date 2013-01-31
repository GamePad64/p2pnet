/*
 * PeerRouteSet.cpp
 *
 *  Created on: 16.01.2013
 *      Author: gamepad
 */

#include "PeerRoute.h"
#include "../relay/RelayTransportSocketEndpoint.h"
#include "../net/UDPTransportSocketEndpoint.h"
#include "../protobuf/TransportSocketEndpoint_s.pb.h"

namespace p2pnet {
namespace databases {

PeerRouteSet::PeerRouteSet() {}
PeerRouteSet::~PeerRouteSet() {}

std::string PeerRouteSet::toString() {
	net::PeerRoute_s peerroute_s;
	for(relay::RelayTransportSocketEndpoint &endpoint_relay : this->endpoint_relay_list){
		peerroute_s.add_endpoint_relay(endpoint_relay.toString());
	}
	for(net::UDPTransportSocketEndpoint &endpoint_udp : this->endpoint_udp_list){
		peerroute_s.add_endpoint_udp(endpoint_udp.toString());
	}
	return peerroute_s.SerializeAsString();
}

void PeerRouteSet::fromString(const std::string &from) {
	net::PeerRoute_s peerroute_s;
	peerroute_s.ParseFromString(from);

	for(const std::string &endpoint_relay_s : peerroute_s.endpoint_relay()){
		this->endpoint_relay_list.push_front(relay::RelayTransportSocketEndpoint(endpoint_relay_s));
	}
	for(const std::string &endpoint_udp_s : peerroute_s.endpoint_udp()){
		this->endpoint_udp_list.push_front(net::UDPTransportSocketEndpoint(endpoint_udp_s));
	}
}

} /* namespace databases */
} /* namespace p2pnet */
