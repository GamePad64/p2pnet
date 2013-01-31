/*
 * TransportSocket.cpp
 *
 *  Created on: 30.01.2013
 *      Author: gamepad
 */

#include "TransportSocket.h"
#include "TransportSocketListener.h"
#include "TransportSocketEndpoint.h"

namespace p2pnet {
namespace net {

TransportSocket::~TransportSocket(){};

MessageBundle TransportSocket::createMessageBundle(std::string message,
		TransportSocketEndpoint &endpoint,
		TransportSocketConnection connection) {
	MessageBundle bundle;
	if (!connection.isNull()) {
		bundle.socket_connection = connection;
	} else {
		TransportSocketConnection socket_connection(this, &endpoint);
		bundle.socket_connection = socket_connection;
	}
	bundle.message = message;
	return bundle;
}

}
}
