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

#include "../TransportSocketEndpoint.h"
#include <memory>
#include <boost/asio.hpp>

namespace p2pnet {
namespace net {

using namespace boost::asio::ip;

class UDPTransportSocketEndpoint: public p2pnet::net::TransportSocketEndpoint {
	friend class UDPTransportSocket;
private:
	udp::endpoint& getEndpoint();
	void setEndpoint(const udp::endpoint& endpoint);
protected:
	udp::endpoint asio_endpoint;
public:
	UDPTransportSocketEndpoint();
	virtual ~UDPTransportSocketEndpoint();

	typedef std::shared_ptr<UDPTransportSocketEndpoint> udp_pointer;

	/*!
	 * This function returns true if IP address is an IPv4 address.
	 * @return bool
	 */
	bool isIPv4();
	/*!
	 * This function returns true if IP address is an IPv6 address.
	 * @return bool
	 */
	bool isIPv6();

	/*!
	 * This function returns IP address of the endpoint.
	 * @return std::string in "255.255.255.255" format (if IPv4) or in hexadecimal IPv6 representation.
	 */
	std::string getIP() const;
	/*!
	 * This function sets IP address of the endpoint
	 * @param ip IP address in IPv4 or IPv6 format.
	 */
	void setIP(const std::string& ip);

	typedef unsigned short int port_t;
	port_t getPort() const;
	void setPort(port_t port);
	/*!
	 * This is a constructor of UDPTransportSocketEndpoint with setting IP and port.
	 * @param ip IP address in IPv4 or IPv6 format.
	 * @param port Port ranging from 1 to 65535.
	 */
	UDPTransportSocketEndpoint(std::string ip, port_t port);

	// Inherited from TransportSocketEndpoint
	virtual TransportSocketEndpoint::pointer yieldCopyPtr() const {
		TransportSocketEndpoint::pointer copy = std::make_shared<UDPTransportSocketEndpoint>(UDPTransportSocketEndpoint(*this));
		return copy;
	}

	virtual std::string toString();
	virtual void fromString(std::string endpoint_s);
	UDPTransportSocketEndpoint(std::string endpoint_s);

	virtual std::string toHRString();
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* UDPTRANSPORTSOCKETENDPOINT_H_ */
