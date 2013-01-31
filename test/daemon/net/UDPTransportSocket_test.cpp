/*
 * UDPSocket_test.cpp
 *
 *  Created on: 07.01.2013
 *      Author: gamepad
 */

#include "gtest/gtest.h"
#include "../../../src/daemon/net/TransportSocketListener.h"
#include "../../../src/daemon/net/UDPTransportSocketEndpoint.h"
#include "../../../src/daemon/net/UDPTransportSocket.h"
#include "../../../src/daemon/AsioIOService.h"

namespace p2pnet {
namespace test {

static std::string message_orig = "message over socket!";
unsigned int port_wait = 15475;
unsigned int port_async = 15476;
unsigned int port_here = 15477;

class MockDataListener : public net::TransportSocketListener {
public:
	MockDataListener(){};
	virtual ~MockDataListener(){};

	std::string message_received;
	virtual void receivedMessage(net::MessageBundle message_bundle){
		message_received = message_bundle.message;
	}
	virtual void sentMessage(net::MessageBundle message_bundle){

	}
};

TEST(UDPTransportSocket, Wait_Send_receive){
	MockDataListener mdo_wait;

	net::UDPTransportSocket u_send;
	net::UDPTransportSocket u_receive;

	net::UDPTransportSocketEndpoint endpoint("127.0.0.1", port_wait);
	u_receive.bindLocalIPv4(port_wait);
	u_send.openIPv4();
	u_send.waitSendTo(endpoint, message_orig, &mdo_wait);
	u_receive.waitReceiveFrom(endpoint, &mdo_wait);

	ASSERT_EQ(message_orig, mdo_wait.message_received);
}

TEST(UDPTransportSocket, Async_Send_receive){
	MockDataListener mdo_async;

	AsioIOService::getIOService().reset();

	net::UDPTransportSocket u_send;
	net::UDPTransportSocket u_receive;

	net::UDPTransportSocketEndpoint endpoint("127.0.0.1", port_async);
	u_receive.bindLocalIPv4(port_async);
	u_receive.asyncReceiveFrom(endpoint, &mdo_async);
	u_send.openIPv4();
	u_send.asyncSendTo(endpoint, message_orig, &mdo_async);

	AsioIOService::getIOService().poll_one();
	AsioIOService::getIOService().poll_one();

	ASSERT_EQ(message_orig, mdo_async.message_received);
}

TEST(UDPTransportSocket, Here_Send_receive){
	net::UDPTransportSocket u_send;
	net::UDPTransportSocket u_receive;

	net::UDPTransportSocketEndpoint endpoint("127.0.0.1", port_here);
	u_receive.bindLocalIPv4(port_here);
	u_send.openIPv4();
	u_send.hereSendTo(endpoint, message_orig);

	ASSERT_EQ(message_orig, u_receive.hereReceiveFrom(endpoint).message);
}

}
}
