/*
 * You may redistribute this program and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gtest/gtest.h"
#include "../../../src/daemon/net/TransportSocketListener.h"
#include "../../../src/daemon/net/TransportSocketEndpoint.h"
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
	u_send.waitSendTo(endpoint, message_orig);
	u_receive.addListener(&mdo_wait);
	u_receive.waitReceiveFrom(endpoint);

	ASSERT_EQ(message_orig, mdo_wait.message_received);
}

TEST(UDPTransportSocket, Async_Send_receive){
	MockDataListener mdo_async;

	AsioIOService::getIOService().reset();

	net::UDPTransportSocket u_send;
	net::UDPTransportSocket u_receive;

	net::UDPTransportSocketEndpoint endpoint("127.0.0.1", port_async);
	u_receive.bindLocalIPv4(port_async);
	u_receive.addListener(&mdo_async);
	u_receive.asyncReceiveFrom(endpoint);
	u_send.openIPv4();
	u_send.asyncSendTo(endpoint, message_orig);

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
