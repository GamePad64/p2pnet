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
#include "../../protobuf/Protocol.pb.h"
#include "../../databases/PersonalKeyStorage.h"

namespace p2pnet {
namespace net {
namespace lpd {

UDPLPD::UDPLPD(Config& config,
		net::UDPTransportSocket& udp_socket,
		databases::NetDBStorage& netdb_storage) :
				m_config(config),
				m_io_service(AsioIOService::getIOService()),
				m_timer(m_io_service),
				m_lpd_socket(m_io_service),
				m_udp_socket(udp_socket),
				m_netdb_storage(netdb_storage) {
	m_target_port = 0;
	m_timer_seconds = 0;
}

UDPLPD::~UDPLPD() {
	m_timer.cancel();
}

void UDPLPD::run() {
	initSocket();
	startReceive();
	startSend();
}

protocol::p2pMessage UDPLPD::generateAgreementMessage() {
	protocol::p2pMessage message;

	protocol::p2pMessageHeader* message_header = message.mutable_message_header();
	databases::PersonalKeyStorage* pks = databases::PersonalKeyStorage::getInstance();
	message_header->set_src_id(std::string(pks->getMyTransportHash().begin(), pks->getMyTransportHash().end()));

	message.set_message_type(message.AGREEMENT);

	protocol::p2pMessage_Agreement message_agreement;
	crypto::key_public_t pubkey = pks->getMyPublicKey();
	std::string pubkey_s = std::string(pubkey.begin(), pubkey.end());
	message_agreement.set_src_pubkey(pubkey_s);
	message.set_message_s(message_agreement.SerializeAsString());

	return message;
}

void UDPLPD::processReceived(size_t bytes,
		std::shared_ptr<ip::udp::endpoint> endpoint,
		char* recv_buffer){
	// We received message, continue receiving others
	receive();

	// Create Protocol Buffer message
	UDPLPDMessage message;
	message.ParseFromString(std::string(recv_buffer, bytes));
	delete[] recv_buffer;

	std::clog << "[" << getServiceName() << "] Local <- " << endpoint->address().to_string() << ":" << message.port() << std::endl;

	std::string received_address = endpoint->address().to_string();
	net::UDPTransportSocketEndpoint received_endpoint(received_address, message.port());
	m_udp_socket.hereSendTo(received_endpoint, generateAgreementMessage().SerializeAsString());
	std::clog << "[" << getServiceName() << "] Sent agreement request to " << received_endpoint.getIP() << std::endl;
}

void UDPLPD::waitBeforeSend() {
	m_timer.expires_from_now(boost::posix_time::seconds(m_timer_seconds));
	m_timer.async_wait(boost::bind(&UDPLPD::send, this));
}

void UDPLPD::send(){
	std::clog << "[" << getServiceName() << "] Local -> " << m_target_address.to_string() << ":" << m_target_port << std::endl;
	m_lpd_socket.async_send_to(buffer(generateLPDMessage().SerializeAsString()), ip::udp::endpoint(m_target_address, m_target_port), boost::bind(&UDPLPD::waitBeforeSend, this));
}

void UDPLPD::receive(){
	char* lpd_packet = new char[2048];
	std::shared_ptr<ip::udp::endpoint> endpoint = std::make_shared<ip::udp::endpoint>(m_bind_address, m_target_port);
	m_lpd_socket.async_receive_from(
			buffer(lpd_packet, 2048), *endpoint,
			boost::bind(&UDPLPD::processReceived, this, placeholders::bytes_transferred, endpoint, lpd_packet));
}

void UDPLPD::startSend() {
	std::clog << "[" << getServiceName() << "] Started sending broadcasts to: " << m_target_address << ":" << m_target_port << std::endl;
	send();
}

void UDPLPD::startReceive() {
	std::clog << "[" << getServiceName() << "] Started receiving broadcasts from: " << m_target_address << ":" << m_target_port << std::endl;
	receive();
}

} /* namespace lpd */
} /* namespace net */
} /* namespace p2pnet */
