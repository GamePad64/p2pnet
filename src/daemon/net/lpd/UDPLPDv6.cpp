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
#include "../UDPTransportSocketEndpoint.h"

namespace p2pnet {
namespace net {
namespace lpd {

UDPLPDv6::UDPLPDv6(Config& config, net::UDPTransportSocket& udp_socket, databases::NetDBStorage& netdb_storage) : UDPLPD(config, udp_socket, netdb_storage) {
	readConfig();
	initSocket();
}
UDPLPDv6::~UDPLPDv6() {
	m_lpd_socket.set_option(ip::multicast::leave_group(m_target_address));
}

UDPLPDMessage UDPLPDv6::generateLPDMessage() {
	UDPLPDMessage message;
	unsigned int my_port = m_config.getConfig().get("net.sockets.udpv6.port", 2185);
	message.set_port(my_port);
	return message;
}

void UDPLPDv6::readConfig() {
	this->m_timer_seconds = m_config.getConfig().get("net.lpd.udpv6.timer", m_default_timer_seconds);
	this->m_target_address = ip::address::from_string(m_config.getConfig().get("net.lpd.udpv6.address", m_default_target_address));
	this->m_target_port = m_config.getConfig().get("net.lpd.udpv6.port", m_default_target_port);
	this->m_bind_address = ip::address::from_string(m_config.getConfig().get("net.lpd.udpv6.address", m_default_bind_address));
}

void UDPLPDv6::initSocket() {
	m_lpd_socket.open(ip::udp::v6());

	m_lpd_socket.set_option(ip::multicast::join_group(m_target_address));
	m_lpd_socket.set_option(ip::multicast::enable_loopback(false));
	m_lpd_socket.set_option(ip::udp::socket::reuse_address(true));
	m_lpd_socket.set_option(ip::v6_only(true));

	m_lpd_socket.bind(ip::udp::endpoint(m_bind_address, m_target_port));
}

} /* namespace lpd */
} /* namespace net */
} /* namespace p2pnet */
