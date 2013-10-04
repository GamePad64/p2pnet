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

using namespace boost::asio::ip;

namespace p2pnet {
namespace transport {

class UDPTransportInterface : public TransportInterface {
	// MTU values from http://stackoverflow.com/a/7072799
	const unsigned short IPv4_MTU = 1438;
	const unsigned short IPv6_MTU = 1280;

	unsigned short mtu;

	udp::endpoint local;

	boost::asio::io_service& m_io_service;
	udp::socket m_socket;
protected:
	void receivedMessageHandler(boost::asio::streambuf* buffer, size_t bytes_received, udp::endpoint* endpoint);
public:
	UDPTransportInterface();
	virtual ~UDPTransportInterface();

	virtual std::shared_ptr<TransportInterfaceEndpoint> createEndpoint();
	virtual std::string getInterfacePrefix() const;

	inline unsigned short getMTU(){
		return mtu;
	}

	void openIPv4();
	void openIPv6();
	void openAll();
	/*!
	 * Opens the socket, sets it to IPv4, binds on all available network interfaces and sets MTU = IPv4_MTU
	 * @param port
	 */
	void bindLocalIPv4(UDPTransportInterfaceEndpoint::port_t port);
	/*!
	 * Opens the socket, sets it to IPv6, binds on all available network interfaces and sets MTU = IPv6_MTU
	 * @param port
	 */
	void bindLocalIPv6(UDPTransportInterfaceEndpoint::port_t port);
	void bindLocalAll(UDPTransportInterfaceEndpoint::port_t port);

	//Inherited from TransportSocket
	virtual uint32_t getInterfaceID() const {return 1;};

	virtual void receive();
	virtual void send(TransportSocketEndpoint endpoint, const std::string& data);
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* UDPTRANSPORTSOCKET_H_ */
