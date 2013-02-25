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

#include "UDPLPD.h"
#include "../../AsioIOService.h"

namespace p2pnet {
namespace net {
namespace lpd {

UDPLPD::UDPLPD(Config& config) : m_config(config), m_io_service(AsioIOService::getIOService()), m_timer(m_io_service),
		m_socket(m_io_service) {
	udp_message = generateMessage().SerializeAsString();
}

UDPLPD::~UDPLPD() {
	m_timer.cancel();
}

void UDPLPD::run() {
	this->initSocket();
	startSend();
}

UDPLPDMessage UDPLPD::generateMessage() {
	UDPLPDMessage message;
	message.set_port(m_target_port);
	return message;
}

void UDPLPD::waitBeforeSend() {
	m_timer.expires_from_now(boost::posix_time::seconds(m_timer_seconds));
	m_timer.async_wait(boost::bind(&UDPLPD::startSend, this));
}

void UDPLPD::startSend() {
	std::clog << "[UDPLPD] Local Peer Discovery message sent to: " << m_target_address.to_string() << " Port: " << m_target_port << std::endl;
	m_socket.async_send_to(buffer(udp_message), ip::udp::endpoint(m_target_address, m_target_port), boost::bind(&UDPLPD::waitBeforeSend, this));
}

} /* namespace lpd */
} /* namespace net */
} /* namespace p2pnet */
