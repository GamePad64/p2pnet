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
#ifndef UDPTRANSPORTCONNECTION_H_
#define UDPTRANSPORTCONNECTION_H_

#include <boost/asio.hpp>
#include <boost/asio/system_timer.hpp>

#define UDP_KEEPALIVE_MSG "UDP_KEEPALIVE"
#define UDP_CONNECT_MSG "UDP_CONNECT"
#define UDP_DISCONNECT_MSG "UDP_DISCONNECT"

#include "../Connection.h"

namespace p2pnet {
namespace transport {

class UDPConnection : public Connection {
	SocketEndpoint endpoint;

	Socket::ConnectCallback connecting_callback;

	bool m_connected = false;
	boost::asio::system_timer conn_send_timer;	// Send "keep-alive"
	boost::asio::system_timer conn_receive_timeout;	// Wait for "keep-alive"

	void sendKeepAlive(const boost::system::error_code& error);
public:
	UDPConnection(SocketEndpoint endpoint) = delete;
	UDPConnection(SocketEndpoint endpoint, std::shared_ptr<Interface> parent_interface);
	virtual ~UDPConnection();

	void receivedConnect();
	void receivedKeepAlive();
	void receivedDisconnect();

	///
	virtual SocketEndpoint getEndpoint(){return endpoint;};

	virtual void connect(Socket::ConnectCallback callback);
	virtual void send(const std::string& data, Socket::SendCallback callback);
	virtual bool connected() const;
};

} /* namespace transport */
} /* namespace p2pnet */

#endif /* UDPTRANSPORTCONNECTION_H_ */
