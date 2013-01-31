/*
 * MessageDispatcher.h
 *
 *  Created on: 17.01.2013
 *      Author: gamepad
 */

#ifndef MESSAGEDISPATCHER_H_
#define MESSAGEDISPATCHER_H_

#include "CryptoHandler.h"
#include "../net/TransportSocket.h"
#include "../protobuf/protocol.pb.h"

namespace p2pnet {
namespace messaging {

class MessageDispatcher : public net::TransportSocketListener {
	CryptoHandler crypto_proc;
public:
	MessageDispatcher();
	virtual ~MessageDispatcher();

	virtual void receivedMessage(net::message_bundle_t message_bundle);
	virtual void sentMessage(net::message_bundle_t message_bundle);
};

} /* namespace messaging */
} /* namespace p2pnet */
#endif /* MESSAGEDISPATCHER_H_ */
