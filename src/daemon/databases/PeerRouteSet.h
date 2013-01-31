/*
 * PeerRoute.h
 *
 *  Created on: 16.01.2013
 *      Author: gamepad
 */

#ifndef PEERROUTESET_H_
#define PEERROUTESET_H_

#include "../../common/crypto/CryptoTypes.h"
#include "../relay/RelayTransportSocketEndpoint.h"
#include "../net/UDPTransportSocketEndpoint.h"
#include <forward_list>

namespace p2pnet {
namespace databases {

/**
 * PeerRouteSet is a class that stores all the ways to reach the destination.
 */
class PeerRouteSet {
public:
	PeerRouteSet();
	virtual ~PeerRouteSet();

	std::forward_list<relay::RelayTransportSocketEndpoint> endpoint_relay_list;
	decltype(endpoint_relay_list)& getRelayEndpointList() {return endpoint_relay_list;};
	std::forward_list<net::UDPTransportSocketEndpoint> endpoint_udp_list;
	decltype(endpoint_udp_list)& getUDPEndpointList() {return endpoint_udp_list;}

	// Protocol Buffers serialization
	std::string toString();
	void fromString(const std::string &from);
};

} /* namespace databases */
} /* namespace p2pnet */
#endif /* PEERROUTESET_H_ */
