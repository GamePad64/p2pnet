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
namespace net {
namespace lpd {

UDPLPDv4::UDPLPDv4(Config& config) : UDPLPD(config) {
	readConfig();
	initSocket();
}
UDPLPDv4::~UDPLPDv4() {
	m_socket.set_option(ip::multicast::leave_group(m_target_address));
}

void UDPLPDv4::readConfig() {
	this->m_timer_seconds = m_config.getConfig().get("net.lpd.udpv4.timer", m_default_timer_seconds);
	this->m_target_address = ip::address::from_string(m_config.getConfig().get("net.lpd.udpv4.address", m_default_target_address));
	this->m_target_port = m_config.getConfig().get("net.lpd.udpv4.port", m_default_target_port);
	this->m_bind_address = ip::address::from_string(m_config.getConfig().get("net.lpd.udpv4.address", m_default_bind_address));
}

void UDPLPDv4::initSocket() {
	m_socket.open(ip::udp::v4());

	m_socket.set_option(ip::multicast::join_group(m_target_address));
	m_socket.set_option(ip::multicast::enable_loopback(false));
}

} /* namespace lpd */
} /* namespace net */
} /* namespace p2pnet */
