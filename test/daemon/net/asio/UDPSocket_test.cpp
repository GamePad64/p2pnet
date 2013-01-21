/*
 * UDPSocket_test.cpp
 *
 *  Created on: 07.01.2013
 *      Author: gamepad
 */

#include "../../../../src/daemon/net/MessageSocket.h"
#include "../../../../src/daemon/net/asio/UDPSocket.h"
#include "../../../../src/daemon/net/asio/AsioIOService.h"
#include "gtest/gtest.h"

namespace p2pnet {
namespace test {

static std::string message_orig = "message over socket!";
unsigned int port_wait = 15475;
unsigned int port_async = 15476;
unsigned int port_here = 15477;

class MockDataObserver : public net::MessageSocketListener {
public:
	std::string message_recv;
	MockDataObserver(){};
	virtual ~MockDataObserver(){};
	virtual void receivedMessage(net::packet_info_t packet){
		message_recv = packet.message;
	}
	virtual void sentMessage(net::packet_info_t packet){
	}
};

TEST(UDPSocket, Wait_Send_receive){
	MockDataObserver mdo_wait;

	net::UDPSocket u_send;
	net::UDPSocket u_receive;

	net::UDPSocketDestination dest("127.0.0.1", port_wait);
	u_receive.bind(net::UDPSocketDestination(net::LOCAL_V4, port_wait));
	u_send.wait_send_to(dest, message_orig, &mdo_wait);
	u_receive.wait_receive_from(dest, &mdo_wait);

	u_send.close();
	u_receive.close();

	ASSERT_EQ(message_orig, mdo_wait.message_recv);
}

TEST(UDPSocket, Async_Send_receive){
	MockDataObserver mdo_async;

	net::UDPSocket u_send;
	net::UDPSocket u_receive;

	net::UDPSocketDestination dest("127.0.0.1", port_async);
	u_receive.bind(net::UDPSocketDestination(net::LOCAL_V4, port_async));
	u_receive.async_receive_from(dest, &mdo_async);
	u_send.async_send_to(dest, message_orig, &mdo_async);

	net::AsioIOService::getIOService().run();

	u_send.close();
	u_receive.close();

	ASSERT_EQ(message_orig, mdo_async.message_recv);
}

TEST(UDPSocket, Here_Send_receive){
	net::UDPSocket u_send;
	net::UDPSocket u_receive;

	net::UDPSocketDestination dest("127.0.0.1", port_async);
	u_receive.bind(dest);
	u_send.here_send_to(dest, message_orig);

	ASSERT_EQ(message_orig, u_receive.here_receive_from(dest).message);

	u_send.close();
	u_receive.close();
}

}
}
