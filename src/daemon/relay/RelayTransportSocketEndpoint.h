/*
 * RelayTransportSocketEndpoint.h
 *
 *  Created on: 31.01.2013
 *      Author: gamepad
 */

#ifndef RELAYTRANSPORTSOCKETENDPOINT_H_
#define RELAYTRANSPORTSOCKETENDPOINT_H_

#include "../../common/crypto/CryptoTypes.h"
#include "../net/TransportSocketEndpoint.h"

namespace p2pnet {
namespace relay {

class RelayTransportSocketEndpoint: public p2pnet::net::TransportSocketEndpoint {
	crypto::hash_t relay_hash;
public:
	RelayTransportSocketEndpoint();
	virtual ~RelayTransportSocketEndpoint();

	const crypto::hash_t& getRelayHash() const;
	void setRelayHash(const crypto::hash_t& relayHash);
	RelayTransportSocketEndpoint(crypto::hash_t relay_hash);

	virtual std::string toString();
	virtual void fromString(std::string endpoint_s);
	RelayTransportSocketEndpoint(std::string endpoint_s);
};

} /* namespace relay */
} /* namespace p2pnet */
#endif /* RELAYTRANSPORTSOCKETENDPOINT_H_ */
