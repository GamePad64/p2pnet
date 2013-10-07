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

#include "UDPLPDv4.h"

namespace p2pnet {
namespace discovery {

UDPLPDv4::UDPLPDv4(ConfigManager& config) : UDPLPD(config) {
	readConfig();

	m_lpd_socket.open(ip::udp::v4());
	m_lpd_socket.set_option(ip::multicast::join_group(target_endpoint.address()));
	m_lpd_socket.set_option(ip::multicast::enable_loopback(false));
	m_lpd_socket.set_option(ip::udp::socket::reuse_address(true));

	m_lpd_socket.bind(bind_endpoint);
}

UDPLPDv4::~UDPLPDv4() {
	m_lpd_socket.set_option(ip::multicast::leave_group(target_endpoint.address()));
}

void UDPLPDv4::readConfig() {
	m_timer_seconds = getValue<unsigned int>("discovery.udpv4.timer");

	unsigned short multicast_port = getValue<unsigned short>("discovery.udpv4.multicast.port");
	bind_endpoint.port(multicast_port);
	target_endpoint.port(multicast_port);

	bind_endpoint.address(ip::address::from_string(getValue<std::string>("discovery.udpv4.local_ip")));
	target_endpoint.address(ip::address::from_string(getValue<std::string>("discovery.udpv4.multicast.ip")));
}

} /* namespace discovery */
} /* namespace p2pnet */
