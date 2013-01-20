/*
 * PeerRoute.h
 *
 *  Created on: 16.01.2013
 *      Author: gamepad
 */

#ifndef PEERROUTE_H_
#define PEERROUTE_H_

#include "../../common/crypto/CryptoTypes.h"
#include "../net/asio/UDPSocket.h"
#include <list>

namespace p2pnet {
namespace net {

/**
 * PeerRoute is a class that stores all the ways to reach the destination.
 */
class PeerRoute {
public:
	PeerRoute();
	virtual ~PeerRoute();

	std::list<crypto::hash_t> dest_indir;
	std::list<UDPSocketDestination> dest_udp;

	// Protocol Buffers serialization
	std::string toString();
	void fromString(const std::string &from);
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* PEERROUTE_H_ */
