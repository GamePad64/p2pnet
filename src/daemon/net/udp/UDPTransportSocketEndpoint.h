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

	bool isIPv4();
	bool isIPv6();

	std::string getIP() const;
	void setIP(const std::string& ip);

	typedef unsigned short int port_t;
	port_t getPort() const;
	void setPort(port_t port);
	UDPTransportSocketEndpoint(std::string ip, port_t port);

	// Inherited from TransportSocketEndpoint
	virtual TransportSocketEndpoint::pointer yieldCopyPtr() const {
		TransportSocketEndpoint::pointer copy = std::make_shared<UDPTransportSocketEndpoint>(UDPTransportSocketEndpoint(*this));
		return copy;
	}

	virtual TransportSocketEndpoint_s toProtobuf();
	UDPTransportSocketEndpoint(TransportSocketEndpoint_s tse_s);

	virtual std::string toHRString();
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* UDPTRANSPORTSOCKETENDPOINT_H_ */
