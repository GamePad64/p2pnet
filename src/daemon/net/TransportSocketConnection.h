/*
 * TransportSocketConnection.h
 *
 *  Created on: 27.01.2013
 *      Author: gamepad
 */

#ifndef TRANSPORTSOCKETCONNECTION_H_
#define TRANSPORTSOCKETCONNECTION_H_

#include "TransportSocketEndpoint.h"
#include <memory>

namespace p2pnet {
namespace net {

class TransportSocket;
class TransportSocketListener;
class MessageBundle;

/*!
 * A class that wraps TransportSocket with a defined endpoint. It is a class that makes sending packets over different sockets more generic.
 */
class TransportSocketConnection {
	TransportSocket* m_socket;
	std::shared_ptr<TransportSocketEndpoint> m_endpoint;
public:
	TransportSocketConnection(TransportSocket* socket, TransportSocketEndpoint* endpoint);
	virtual ~TransportSocketConnection();

	std::shared_ptr<TransportSocketEndpoint> getEndpointPtr(){return m_endpoint;};

	void asyncReceiveFrom(TransportSocketListener* listener);
	void asyncSendTo(const std::string data, TransportSocketListener* listener);

	void waitReceiveFrom(TransportSocketListener* listener);
	void waitSendTo(const std::string data, TransportSocketListener* listener);

	MessageBundle hereReceiveFrom();
	void hereSendTo(const std::string data);

	bool isNull(){return (m_socket != 0) && (m_endpoint != 0);}
};

} /* namespace net */
} /* namespace p2pnet */

#endif /* TRANSPORTSOCKETCONNECTION_H_ */
