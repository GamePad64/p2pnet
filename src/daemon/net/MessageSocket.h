/*
 * MessageSocket.h
 *
 *  Created on: 17.12.2012
 *      Author: gamepad
 */

#ifndef MESSAGESOCKET_H_
#define MESSAGESOCKET_H_

#include <string>

namespace p2pnet {
namespace net {

constexpr int MAX_PACKET_LENGTH = 32768;

class MessageSocket;
struct packet_info_t {
	net::MessageSocket* socket_ptr;
	std::string message;
};

class MessageSocketListener {
public:
	MessageSocketListener(){};
	virtual ~MessageSocketListener(){};
	virtual void receivedMessage(packet_info_t message) = 0;
	virtual void sentMessage(packet_info_t message) = 0;
};

class MessageSocket {
public:
	MessageSocket();
	virtual ~MessageSocket();

	virtual void async_receive(MessageSocketListener* observer) = 0;
	virtual void async_send(std::string data, MessageSocketListener* observer) = 0;

	virtual void wait_receive(MessageSocketListener* observer) = 0;
	virtual void wait_send(std::string data, MessageSocketListener* observer) = 0;

	virtual packet_info_t here_receive() = 0;
	virtual void here_send(std::string data) = 0;
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* MESSAGESOCKET_H_ */
