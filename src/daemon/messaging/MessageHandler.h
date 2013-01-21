/*
 * MessageHandler.h
 *
 *  Created on: 20.01.2013
 *      Author: gamepad
 */

#ifndef MESSAGEHANDLER_H_
#define MESSAGEHANDLER_H_

#include "../protobuf/protocol.pb.h"
#include "../net/MessageSocket.h"

namespace p2pnet {
namespace messaging {

class MessageHandler {
public:
	MessageHandler(){};
	virtual ~MessageHandler(){};

	virtual void receivedMessage(protocol::p2pMessage p2pmessage_struct, net::packet_info_t message) = 0;
	virtual void sentMessage(protocol::p2pMessage p2pmessage_struct, net::packet_info_t message) = 0;
};

} /* namespace messaging */
} /* namespace p2pnet */
#endif /* MESSAGEHANDLER_H_ */
