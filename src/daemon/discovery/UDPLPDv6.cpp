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

#include "UDPLPDv6.h"

namespace p2pnet {
namespace discovery {

UDPLPDv6::UDPLPDv6(ConfigManager& config) : UDPLPD(config) {
	readConfig();

	m_lpd_socket.open(ip::udp::v6());
	m_lpd_socket.set_option(ip::multicast::join_group(target_endpoint));
	m_lpd_socket.set_option(ip::multicast::enable_loopback(false));
	m_lpd_socket.set_option(ip::udp::socket::reuse_address(true));
	m_lpd_socket.set_option(ip::v6_only(true));

	m_lpd_socket.bind(bind_endpoint);
}

UDPLPDv6::~UDPLPDv6() {
	m_lpd_socket.set_option(ip::multicast::leave_group(target_endpoint.address()));
}

void UDPLPDv6::readConfig() {
	m_timer_seconds = getValue<unsigned int>("discovery.udpv6.timer");

	unsigned short multicast_port = getValue<unsigned short>("discovery.udpv6.multicast.port");
	bind_endpoint.port(multicast_port);
	target_endpoint.port(multicast_port);

	bind_endpoint.address(ip::address::from_string(getValue<std::string>("discovery.udpv6.local_ip")));
	target_endpoint.address(ip::address::from_string(getValue<std::string>("discovery.udpv6.multicast.ip")));
}

} /* namespace discovery */
} /* namespace p2pnet */
