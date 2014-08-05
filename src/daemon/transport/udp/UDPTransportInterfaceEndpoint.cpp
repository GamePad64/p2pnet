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

#include "UDPTransportInterfaceEndpoint.h"
#include "IPInterfaceEndpoint.pb.h"
#include <sstream>
#include <algorithm>

namespace p2pnet {
namespace transport {

UDPTransportInterfaceEndpoint::UDPTransportInterfaceEndpoint(){}
UDPTransportInterfaceEndpoint::UDPTransportInterfaceEndpoint(udp::endpoint rendpoint) : asio_endpoint(rendpoint){}
UDPTransportInterfaceEndpoint::~UDPTransportInterfaceEndpoint(){}

TransportInterfaceEndpoint& UDPTransportInterfaceEndpoint::operator =(const TransportInterfaceEndpoint& rvalue){
	auto pTIE = dynamic_cast<const UDPTransportInterfaceEndpoint&>(rvalue);
	asio_endpoint = pTIE.asio_endpoint;
	return *this;
}

udp::endpoint UDPTransportInterfaceEndpoint::getEndpoint(){
	return this->asio_endpoint;
}

void UDPTransportInterfaceEndpoint::setEndpoint(const udp::endpoint& endpoint){
	this->asio_endpoint = endpoint;
}

bool UDPTransportInterfaceEndpoint::isIPv4(){
	return this->asio_endpoint.address().is_v4();
}

bool UDPTransportInterfaceEndpoint::isIPv6(){
	return this->asio_endpoint.address().is_v6();
}

std::string UDPTransportInterfaceEndpoint::getIP() const {
	return this->asio_endpoint.address().to_string();
}
void UDPTransportInterfaceEndpoint::setIP(const std::string& ip){
	this->asio_endpoint.address(address::from_string(ip.substr(0,ip.find('%'))));
}

UDPTransportInterfaceEndpoint::port_t UDPTransportInterfaceEndpoint::getPort() const {
	return this->asio_endpoint.port();
}
void UDPTransportInterfaceEndpoint::setPort(UDPTransportInterfaceEndpoint::port_t port){
	this->asio_endpoint.port(port);
}
UDPTransportInterfaceEndpoint::UDPTransportInterfaceEndpoint(std::string ip, UDPTransportInterfaceEndpoint::port_t port){
	this->setIP(ip);
	this->setPort(port);
}

void UDPTransportInterfaceEndpoint::fromProtobuf(proto::TransportSocketEndpoint_s tse_s){
	setIP(tse_s.GetExtension(proto::ip));
	setPort(tse_s.GetExtension(proto::port));
}
proto::TransportSocketEndpoint_s UDPTransportInterfaceEndpoint::toProtobuf() const {
	proto::TransportSocketEndpoint_s tse_s;
	tse_s.set_interface_id(getInterfaceID());
	tse_s.SetExtension(proto::ip, getIP());
	tse_s.SetExtension(proto::port, getPort());
	return tse_s;
}

std::string UDPTransportInterfaceEndpoint::toReadableString() const {
	std::ostringstream ss;
	ss << getIP() << ":" << getPort();
	return ss.str();
}
void UDPTransportInterfaceEndpoint::fromReadableString(std::string readable_string){
	std::string port_s, ip;

	auto delim_pos = readable_string.rfind(':');
	port_s = readable_string.substr(delim_pos+1, readable_string.length()-delim_pos);
	setPort(atoi(port_s.c_str()));
	setIP(readable_string.substr(0, delim_pos));
}

} /* namespace net */
} /* namespace p2pnet */
