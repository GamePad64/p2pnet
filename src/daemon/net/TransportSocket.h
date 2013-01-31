/*
 * MessageSocket.h
 *
 *  Created on: 17.12.2012
 *      Author: gamepad
 */

#ifndef MESSAGESOCKET_H_
#define MESSAGESOCKET_H_

#include "TransportSocketConnection.h"

#include <string>

namespace p2pnet {
namespace net {

class TransportSocketListener;
class TransportSocketEndpoint;

class TransportSocket {
protected:
	size_t max_packet_length;

	MessageBundle createMessageBundle(std::string message, TransportSocketEndpoint &endpoint, TransportSocketConnection connection);
public:
	virtual ~TransportSocket();

	virtual void asyncReceiveFrom(TransportSocketEndpoint &endpoint, TransportSocketListener* listener, TransportSocketConnection connection = TransportSocketConnection(0, 0)) = 0;
	virtual void asyncSendTo(TransportSocketEndpoint &endpoint, const std::string data, TransportSocketListener* listener, TransportSocketConnection connection = TransportSocketConnection(0, 0)) = 0;

	virtual void waitReceiveFrom(TransportSocketEndpoint &endpoint, TransportSocketListener* listener, TransportSocketConnection connection = TransportSocketConnection(0, 0)) = 0;
	virtual void waitSendTo(TransportSocketEndpoint &endpoint, const std::string data, TransportSocketListener* listener, TransportSocketConnection connection = TransportSocketConnection(0, 0)) = 0;

	virtual MessageBundle hereReceiveFrom(TransportSocketEndpoint &endpoint, TransportSocketConnection connection = TransportSocketConnection(0, 0)) = 0;
	virtual void hereSendTo(TransportSocketEndpoint &endpoint, const std::string data) = 0;

	size_t getMaxPacketLength() const {return max_packet_length;};
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* MESSAGESOCKET_H_ */
