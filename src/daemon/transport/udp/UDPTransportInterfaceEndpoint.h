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

#ifndef UDPTRANSPORTSOCKETENDPOINT_H_
#define UDPTRANSPORTSOCKETENDPOINT_H_

#include "../TransportInterface.h"
#include <memory>
#include <boost/asio.hpp>

namespace p2pnet {
namespace transport {

using namespace boost::asio::ip;

class UDPTransportInterfaceEndpoint: public TransportInterfaceEndpoint {
	friend class UDPTransportInterface;
private:
	udp::endpoint& getEndpoint();
	void setEndpoint(const udp::endpoint& endpoint);
protected:
	udp::endpoint asio_endpoint;
public:
	UDPTransportInterfaceEndpoint();
	UDPTransportInterfaceEndpoint(udp::endpoint rendpoint);
	virtual ~UDPTransportInterfaceEndpoint();

	virtual TransportInterfaceEndpoint& operator =(const TransportInterfaceEndpoint& rvalue);

	bool isIPv4();
	bool isIPv6();

	std::string getIP() const;
	void setIP(const std::string& ip);

	typedef unsigned short int port_t;
	port_t getPort() const;
	void setPort(port_t port);
	UDPTransportInterfaceEndpoint(std::string ip, port_t port);

	// Inherited from TransportSocketEndpoint
	virtual uint32_t getInterfaceID() const {return 1;}

	virtual void fromProtobuf(databases::TransportSocketEndpoint_s tse_s);
	virtual databases::TransportSocketEndpoint_s toProtobuf() const;

	virtual std::string toReadableString() const;
	virtual void fromReadableString(std::string readable_string);
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* UDPTRANSPORTSOCKETENDPOINT_H_ */
