/*
 * DataObserver.h
 *
 *  Created on: 01.01.2013
 *      Author: gamepad
 */

#ifndef DATAOBSERVER_H_
#define DATAOBSERVER_H_

#include "MessageSocket.h"
#include <string>

namespace p2pnet {

namespace net {
class MessageSocket;
}

struct packet_info_t {
	net::MessageSocket* socket_ptr;
	std::string message;
};

class DataObserver {
public:
	DataObserver(){};
	virtual ~DataObserver(){};
	virtual void receivedMessage(packet_info_t message) = 0;
	virtual void sentMessage(packet_info_t message) = 0;
};

} /* namespace p2pnet */
#endif /* DATAOBSERVER_H_ */
