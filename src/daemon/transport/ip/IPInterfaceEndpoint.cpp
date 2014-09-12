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

#include "IPInterfaceEndpoint.h"
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <algorithm>

namespace p2pnet {
namespace transport {

IPInterfaceEndpoint::IPInterfaceEndpoint(const std::string& prefix) :
		InterfaceEndpoint(prefix) {}
IPInterfaceEndpoint::IPInterfaceEndpoint(address ip, port_t port, const std::string& prefix) :
		InterfaceEndpoint(prefix), ip(ip), port(port) {}
IPInterfaceEndpoint::IPInterfaceEndpoint(const std::string& ip, port_t port, const std::string& prefix) :
		IPInterfaceEndpoint(address::from_string(ip), port, prefix){}

bool IPInterfaceEndpoint::isIPv4() const {
	return ip.is_v4();
}

bool IPInterfaceEndpoint::isIPv6() const {
	return ip.is_v6();
}

std::string IPInterfaceEndpoint::getIPString() const {
	return ip.to_string();
}
void IPInterfaceEndpoint::setIPString(const std::string& ip){
	this->ip = address::from_string(ip.substr(0,ip.find('%')));
}

address IPInterfaceEndpoint::getAsioIP() const {
	return ip;
}
void IPInterfaceEndpoint::setAsioIP(const address& ip){
	this->ip = ip;
}

IPInterfaceEndpoint::port_t IPInterfaceEndpoint::getPort() const {
	return port;
}
void IPInterfaceEndpoint::setPort(IPInterfaceEndpoint::port_t port){
	this->port = port;
}

std::string IPInterfaceEndpoint::toString() const {
	std::ostringstream ss;
	ss << getPrefix() << ":" << getIPString() << ":" << getPort();
	return ss.str();
}
void IPInterfaceEndpoint::fromString(const std::string& string){
	std::string port_s, ip;

	auto prefix_delim = string.find(':');
	auto port_delim = string.rfind(':');
	port = boost::lexical_cast<port_t>(string.substr(port_delim+1, string.length()-port_delim));
	setIPString(string.substr(prefix_delim+1, port_delim-prefix_delim-1));
}

} /* namespace net */
} /* namespace p2pnet */
