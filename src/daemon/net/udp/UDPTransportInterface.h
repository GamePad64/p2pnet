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

#include "../TransportSocket.h"
#include "UDPTransportInterfaceEndpoint.h"
#include <boost/asio.hpp>
#include <memory>

using namespace boost::asio;

namespace p2pnet {
namespace net {

class UDPTransportSocket : public p2pnet::net::TransportSocket {
	const unsigned short IPv4_MTU = 512;
	const unsigned short IPv6_MTU = 1024;

	io_service& m_io_service;
	ip::udp::socket m_socket;
protected:
	void receivedMessageHandler(const std::string& message, TransportSocketEndpoint::pointer endpoint);
	void receivedMessageHandler(char* buffer, size_t bytes_received, TransportSocketEndpoint::pointer endpoint);
	void sentMessageHandler(const std::string& message, TransportSocketEndpoint::pointer endpoint);
	void sentMessageHandler(char* buffer, size_t bytes_sent, TransportSocketEndpoint::pointer endpoint);
public:
	UDPTransportSocket();
	virtual ~UDPTransportSocket();

	void openIPv4();
	void openIPv6();
	void openAll();
	/*!
	 * Opens the socket, sets it to IPv4, binds on all available network interfaces and sets MTU = IPv4_MTU
	 * @param port
	 */
	void bindLocalIPv4(UDPTransportSocketEndpoint::port_t port);
	/*!
	 * Opens the socket, sets it to IPv6, binds on all available network interfaces and sets MTU = IPv6_MTU
	 * @param port
	 */
	void bindLocalIPv6(UDPTransportSocketEndpoint::port_t port);
	void bindLocalAll(UDPTransportSocketEndpoint::port_t port);

	//Inherited from TransportSocket
	virtual void asyncReceiveFrom(TransportSocketEndpoint::pointer endpoint);
	virtual void waitReceiveFrom(TransportSocketEndpoint::pointer endpoint);
	virtual TransportInterfaceCallback hereReceiveFrom(TransportSocketEndpoint::pointer endpoint);

	virtual void asyncSendTo(TransportSocketEndpoint::pointer endpoint, const std::string& data);


	virtual void waitSendTo(TransportSocketEndpoint::pointer endpoint, const std::string& data);


	virtual void hereSendTo(TransportSocketEndpoint::pointer endpoint, const std::string& data);
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* UDPTRANSPORTSOCKET_H_ */
