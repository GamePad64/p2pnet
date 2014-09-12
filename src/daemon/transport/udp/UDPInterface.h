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

#ifndef UDPTRANSPORTSOCKET_H_
#define UDPTRANSPORTSOCKET_H_

#include "../Socket.h"
#include "../Interface.h"
#include "UDPInterfaceEndpoint.h"
#include <boost/asio.hpp>
#include <memory>

using namespace boost::asio::ip;

namespace p2pnet {
namespace transport {

#define MAX_UDP_PACKET_SIZE 64 * 1024
class UDPConnection;
class UDPInterface : public Interface {
	using charbuf_t = std::array<char, MAX_UDP_PACKET_SIZE>;
	using charbuf_ptr_t = std::shared_ptr<charbuf_t>;

	unsigned short mtu;	// MTU value is for future use. Maybe, we will treat it correctly some day.
	int keepalive_timer, inactivity_timeout;

	udp::socket m_socket;
	udp::endpoint local;

	std::set<std::shared_ptr<UDPConnection>> zygote_connections;

	// Callbacks
	Socket::ReceiveCallback receive_callback;
	Socket::DisconnectCallback disconnect_callback;
protected:
	void handleReceive(const boost::system::error_code& error, charbuf_ptr_t buffer,
				size_t bytes_received,
				std::shared_ptr<udp::endpoint> endpoint);
	void handleSend(Socket::SendCallback callback, const boost::system::error_code& error, std::shared_ptr<Connection> connection, std::string message, int bytes);
public:
	UDPInterface();
	virtual ~UDPInterface();

	void receive();
	void send(std::shared_ptr<Connection> connection, const std::string& data, Socket::SendCallback callback);

	// Getters
	int getInactivityTimeout() const {return inactivity_timeout;}
	int getKeepaliveTimer() const {return keepalive_timer;}

	// Inherited functions
	virtual std::string getPrefix() const {return "UDP";}
	virtual bool isLocalOnly() const {return false;}

	virtual std::shared_ptr<InterfaceEndpoint> createEndpoint();

	// Interface asynchronous I/O functions.
	virtual void setReceiveCallback(Socket::ReceiveCallback callback);
	virtual void setDisconnectCallback(Socket::DisconnectCallback callback);
	Socket::DisconnectCallback getDisconnectCallback() {return disconnect_callback;}

	virtual void connect(SocketEndpoint endpoint, Socket::ConnectCallback callback);
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* UDPTRANSPORTSOCKET_H_ */
