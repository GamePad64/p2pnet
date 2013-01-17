/*
 * MessageDispatcher.h
 *
 *  Created on: 17.01.2013
 *      Author: gamepad
 */

#ifndef MESSAGEDISPATCHER_H_
#define MESSAGEDISPATCHER_H_

#include "../net/MessageSocket.h"
#include "../protobuf/protocol.pb.h"

namespace p2pnet {
namespace messaging {

class MessageDispatcher : public net::MessageSocketListener {
public:
	MessageDispatcher();
	virtual ~MessageDispatcher();

	virtual void receivedMessage(net::packet_info_t message);
	virtual void sentMessage(net::packet_info_t message);
};

} /* namespace messaging */
} /* namespace p2pnet */
#endif /* MESSAGEDISPATCHER_H_ */
