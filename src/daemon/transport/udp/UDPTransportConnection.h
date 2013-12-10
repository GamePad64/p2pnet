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

#define KEEP_ALIVE_MSG "UDP_KEEP_ALIVE"
#define SEND_TIMER 5
#define RECEIVE_TIMEOUT 10

#include "../TransportConnection.h"

namespace p2pnet {
namespace transport {

class UDPTransportInterface;

class UDPTransportConnection : public TransportConnection {
	UDPTransportInterface* m_parent_interface;

	bool m_connected = false;
	boost::asio::deadline_timer conn_send_timer;	// Send "keep-alive" every 5 seconds.
	boost::asio::deadline_timer conn_receive_timeout;	// Wait for "keep-alive" every 10 seconds.

	void sendKeepAlive();
public:
	UDPTransportConnection(TransportSocketEndpoint endpoint) = delete;
	UDPTransportConnection(TransportSocketEndpoint endpoint, UDPTransportInterface* parent_interface);
	virtual ~UDPTransportConnection();

	virtual void send(std::string data);
	virtual void process(std::string data);
	virtual bool connected() const;
};

} /* namespace transport */
} /* namespace p2pnet */

#endif /* UDPTRANSPORTCONNECTION_H_ */
