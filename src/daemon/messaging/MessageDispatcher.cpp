/*
 * MessageDispatcher.cpp
 *
 *  Created on: 17.01.2013
 *      Author: gamepad
 */

#include "MessageDispatcher.h"

namespace p2pnet {
namespace messaging {

MessageDispatcher::MessageDispatcher() {
	// TODO Auto-generated constructor stub

}

MessageDispatcher::~MessageDispatcher() {
	// TODO Auto-generated destructor stub
}

void MessageDispatcher::receivedMessage(net::packet_info_t message) {
	protocol::p2pMessage p2pmessage_struct;
	p2pmessage_struct.ParseFromString(message.message);

	switch(p2pmessage_struct.message_type()){
	case p2pmessage_struct.AGREEMENT:
		crypto_proc.receivedMessage(p2pmessage_struct, message);
		break;

	default:
		break;
	}
}

void MessageDispatcher::sentMessage(net::packet_info_t message) {
}

} /* namespace messaging */
} /* namespace p2pnet */
