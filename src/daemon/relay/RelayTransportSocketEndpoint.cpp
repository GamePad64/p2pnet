/*
 * RelayTransportSocketEndpoint.cpp
 *
 *  Created on: 31.01.2013
 *      Author: gamepad
 */

#include "RelayTransportSocketEndpoint.h"

namespace p2pnet {
namespace relay {

RelayTransportSocketEndpoint::RelayTransportSocketEndpoint() {}
RelayTransportSocketEndpoint::~RelayTransportSocketEndpoint() {}

const crypto::hash_t& RelayTransportSocketEndpoint::getRelayHash() const {
	return relay_hash;
}

void RelayTransportSocketEndpoint::setRelayHash(
		const crypto::hash_t& relayHash) {
	relay_hash = relayHash;
}

RelayTransportSocketEndpoint::RelayTransportSocketEndpoint(
		crypto::hash_t relay_hash) {
	this->relay_hash = relay_hash;
}

std::string RelayTransportSocketEndpoint::toString() {
	return std::string(relay_hash.begin(), relay_hash.end());
}

void RelayTransportSocketEndpoint::fromString(std::string endpoint_s) {
	relay_hash = crypto::hash_t(endpoint_s.begin(), endpoint_s.end());
}

RelayTransportSocketEndpoint::RelayTransportSocketEndpoint(
		std::string endpoint_s) {
	this->fromString(endpoint_s);
}

} /* namespace relay */
} /* namespace p2pnet */
