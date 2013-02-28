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
#include "../UDPTransportSocket.h"

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
	initSocket();
	startReceive();
	startSend();
}

UDPLPDMessage UDPLPD::generateMessage() {
	UDPLPDMessage message;
	unsigned int my_port = m_config.getConfig().get("net.sockets.udpv4.port", 2185);
	message.set_port(my_port);
	return message;
}

void UDPLPD::processReceived(size_t bytes, std::shared_ptr<ip::udp::endpoint> endpoint, char* recv_buffer){
	receive();
	UDPLPDMessage message;
	message.ParseFromString(std::string(recv_buffer, bytes));
	std::clog << "[UDPLPD] Local <- " << endpoint->address().to_string() << ":" << message.port() << std::endl;
	delete[] recv_buffer;
}

void UDPLPD::waitBeforeSend() {
	m_timer.expires_from_now(boost::posix_time::seconds(m_timer_seconds));
	m_timer.async_wait(boost::bind(&UDPLPD::send, this));
}

void UDPLPD::send(){
	std::clog << "[UDPLPD] Local -> " << m_target_address.to_string() << ":" << m_target_port << std::endl;
	m_socket.async_send_to(buffer(udp_message), ip::udp::endpoint(m_target_address, m_target_port), boost::bind(&UDPLPD::waitBeforeSend, this));
}

void UDPLPD::receive(){
	char* lpd_packet = new char[2048];
	std::shared_ptr<ip::udp::endpoint> endpoint = std::make_shared<ip::udp::endpoint>(m_bind_address, m_target_port);
	m_socket.async_receive_from(
			buffer(lpd_packet, 2048), *endpoint,
			boost::bind(&UDPLPD::processReceived, this, placeholders::bytes_transferred, endpoint, lpd_packet));
}

void UDPLPD::startSend() {
	std::clog << "[UDPLPD] Started sending broadcasts to: " << m_target_address << ":" << m_target_port << std::endl;
	send();
}

void UDPLPD::startReceive() {
	std::clog << "[UDPLPD] Started receiving broadcasts from: " << m_target_address << ":" << m_target_port << std::endl;
	receive();
}

} /* namespace lpd */
} /* namespace net */
} /* namespace p2pnet */
