/*
 * MessageSocket.h
 *
 *  Created on: 17.12.2012
 *      Author: gamepad
 */

#ifndef MESSAGESOCKET_H_
#define MESSAGESOCKET_H_

#include "DataObserver.h"

namespace p2pnet {
namespace net {

constexpr int MAX_PACKET_LENGTH = 32768;

class MessageSocket {
public:
	MessageSocket();
	virtual ~MessageSocket();

	virtual void async_receive(DataObserver* observer) = 0;
	virtual void async_continious_receive(DataObserver* observer) = 0;
	virtual void async_send(std::string data, DataObserver* observer) = 0;

	virtual void wait_receive(DataObserver* observer) = 0;
	virtual void wait_send(std::string data, DataObserver* observer) = 0;

	virtual packet_info_t here_receive() = 0;
	virtual void here_send(std::string data) = 0;
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* MESSAGESOCKET_H_ */
