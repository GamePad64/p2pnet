/*
 * TransportSocketConnection.cpp
 *
 *  Created on: 31.01.2013
 *      Author: gamepad
 */

#include "TransportSocket.h"
#include "TransportSocketListener.h"
#include "TransportSocketConnection.h"

namespace p2pnet {
namespace net {

TransportSocketConnection::TransportSocketConnection(
		TransportSocket* socket, TransportSocketEndpoint* endpoint) : m_socket(socket), m_endpoint(endpoint) {}

TransportSocketConnection::~TransportSocketConnection() {}

void TransportSocketConnection::asyncReceiveFrom(
		TransportSocketListener* listener) {
	m_socket->asyncReceiveFrom(*m_endpoint, listener, *this);
}

void TransportSocketConnection::asyncSendTo(const std::string data,
		TransportSocketListener* listener) {
	m_socket->asyncSendTo(*m_endpoint, data, listener, *this);
}

void TransportSocketConnection::waitReceiveFrom(
		TransportSocketListener* listener) {
	m_socket->waitReceiveFrom(*m_endpoint, listener, *this);
}

void TransportSocketConnection::waitSendTo(const std::string data,
		TransportSocketListener* listener) {
	m_socket->waitSendTo(*m_endpoint, data, listener, *this);
}

MessageBundle TransportSocketConnection::hereReceiveFrom() {
	return m_socket->hereReceiveFrom(*m_endpoint, *this);
}

void TransportSocketConnection::hereSendTo(const std::string data) {
	m_socket->hereSendTo(*m_endpoint, data);
}

} /* namespace net */
} /* namespace p2pnet */
