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
class UDPSocket;

struct packet_info_t {
	UDPSocket* socket_ptr;
	std::string destination;	//! Now it is UDPSocketDestination in serialized form. Further, it will be used for other types of destinations.

	std::string message;
};

class MessageSocketListener {
public:
	MessageSocketListener(){};
	virtual ~MessageSocketListener(){};
	virtual void receivedMessage(packet_info_t message) = 0;
	virtual void sentMessage(packet_info_t message) = 0;
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* MESSAGESOCKET_H_ */
