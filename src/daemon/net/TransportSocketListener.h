/*
 * TransportSocketListener.h
 *
 *  Created on: 28.01.2013
 *      Author: gamepad
 */

#ifndef TRANSPORTSOCKETLISTENER_H_
#define TRANSPORTSOCKETLISTENER_H_

#include "TransportSocketConnection.h"
#include <memory>
#include <string>

namespace p2pnet {
namespace net {

class MessageBundle {
public:
	MessageBundle() : socket_connection(0, 0){};
	TransportSocketConnection socket_connection;

	std::string message;
};

class TransportSocketListener {
public:
	TransportSocketListener(){};
	virtual ~TransportSocketListener(){};
	virtual void receivedMessage(MessageBundle message_bundle) = 0;
	virtual void sentMessage(MessageBundle message_bundle) = 0;
};
} /* namespace net */
} /* namespace p2pnet */
#endif /* TRANSPORTSOCKETLISTENER_H_ */
