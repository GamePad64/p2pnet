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

#include "UDPTransportSocketEndpoint.h"
#include <sstream>

namespace p2pnet {
namespace net {

UDPTransportSocketEndpoint::UDPTransportSocketEndpoint(){};
UDPTransportSocketEndpoint::~UDPTransportSocketEndpoint(){};

udp::endpoint& UDPTransportSocketEndpoint::getEndpoint(){
	return this->asio_endpoint;
};

void UDPTransportSocketEndpoint::setEndpoint(const udp::endpoint& endpoint){
	this->asio_endpoint = endpoint;
};

bool UDPTransportSocketEndpoint::isIPv4(){
	return this->asio_endpoint.address().is_v4();
};

bool UDPTransportSocketEndpoint::isIPv6(){
	return this->asio_endpoint.address().is_v6();
};

std::string UDPTransportSocketEndpoint::getIP() const {
	return this->asio_endpoint.address().to_string();
};
void UDPTransportSocketEndpoint::setIP(const std::string& ip){
	this->asio_endpoint.address(address::from_string(ip.substr(0,ip.find('%'))));
};

UDPTransportSocketEndpoint::port_t UDPTransportSocketEndpoint::getPort() const {
	return this->asio_endpoint.port();
};
void UDPTransportSocketEndpoint::setPort(UDPTransportSocketEndpoint::port_t port){
	this->asio_endpoint.port(port);
};
UDPTransportSocketEndpoint::UDPTransportSocketEndpoint(std::string ip, UDPTransportSocketEndpoint::port_t port){
	this->setIP(ip);
	this->setPort(port);
};

databases::TransportSocketEndpoint_s UDPTransportSocketEndpoint::toProtobuf() const {
	databases::TransportSocketEndpoint_s tse_s;
	tse_s.set_type(tse_s.UDP);
	tse_s.set_ip(getIP());
	tse_s.set_port(getPort());
	return tse_s;
}

UDPTransportSocketEndpoint::UDPTransportSocketEndpoint(databases::TransportSocketEndpoint_s tse_s) {
	setIP(tse_s.ip());
	setPort(tse_s.port());
}

std::string UDPTransportSocketEndpoint::toHRString(){
	std::ostringstream os;
	os << "UDP:" << getIP() << ":" << getPort();
	return os.str();
}

} /* namespace net */
} /* namespace p2pnet */
