/*
 * PeerRoute.h
 *
 *  Created on: 16.01.2013
 *      Author: gamepad
 */

#ifndef PEERROUTE_H_
#define PEERROUTE_H_

#include "../../common/crypto/CryptoTypes.h"
#include "../UDPTransportSocketEndpoint.h"
#include <list>

namespace p2pnet {
namespace databases {

/**
 * PeerRoute is a class that stores all the ways to reach the destination.
 */
class PeerRoute {
public:
	PeerRoute();
	virtual ~PeerRoute();

	std::list<crypto::hash_t> dest_indir;
	std::list<net::UDPTransportSocketEndpoint> dest_udp;

	// Protocol Buffers serialization
	std::string toString();
	void fromString(const std::string &from);
};

} /* namespace databases */
} /* namespace p2pnet */
#endif /* PEERROUTE_H_ */
