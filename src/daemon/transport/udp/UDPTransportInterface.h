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

// MTU values from http://stackoverflow.com/a/7072799
#define IPv4_MTU 1438
#define IPv6_MTU 1280

#define MAX_UDP_PACKET_SIZE 64 * 1024

class UDPTransportInterface : public TransportInterface {
	unsigned short mtu;	// MTU value is for future use. Maybe, we will treat it correctly some day.

	udp::socket m_socket;
	udp::endpoint local;

	void readConfig();
protected:
	void sentMessageHandler(std::string data, std::shared_ptr<udp::endpoint> endpoint);
	void receivedMessageHandler(char* buffer,
			size_t bytes_received,
			std::shared_ptr<udp::endpoint> endpoint);
public:
	UDPTransportInterface();
	virtual ~UDPTransportInterface();

	virtual std::shared_ptr<TransportInterfaceEndpoint> createEndpoint();
	virtual std::string getInterfacePrefix() const;

	void open();
	void bind(address ip, UDPTransportInterfaceEndpoint::port_t port);

	//Inherited from TransportSocket
	virtual uint32_t getInterfaceID() const {return 1;};

	virtual void receive();
	virtual void send(TransportSocketEndpoint endpoint, const std::string& data);
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* UDPTRANSPORTSOCKET_H_ */
