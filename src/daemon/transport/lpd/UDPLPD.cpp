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
#include "../udp/UDPTransportInterface.h"
#include "../../protobuf/Protocol.pb.h"
#include "../../databases/PersonalKeyStorage.h"
#include "../../messaging/MessageGenerator.h"
#include "../../messaging/SessionMap.h"
#include "../../messaging/PeerProcessor.h"
#include "../../peer/TH.h"
#include <ctime>

namespace p2pnet {
namespace transport {
namespace lpd {

UDPLPD::UDPLPD(ConfigManager& config) : GenericLPD(config), m_config(config), m_io_service(AsioIOService::getIOService()), m_timer(m_io_service), m_lpd_socket(
				m_io_service) {
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

void UDPLPD::waitBeforeSend() {
	m_timer.expires_from_now(boost::posix_time::seconds(m_timer_seconds));
	m_timer.async_wait(boost::bind(&UDPLPD::send, this));
}

bool UDPLPD::reject(messaging::Reason reason) {
	throw(new messaging::RejectException(reason, getComponentName()));
}

bool UDPLPD::reject(messaging::Reason reason, std::string comment) {
	throw(new messaging::RejectException(reason, comment, getComponentName()));
}

void UDPLPD::processReceived(size_t bytes, std::shared_ptr< ip::udp::endpoint > asio_endpoint, char* recv_buffer) {
	// Create Protocol Buffer message
	messaging::protocol::UDPLPDMessage message;
	message.ParseFromString(std::string(recv_buffer, bytes));
	// EXCEPTION: Corrupted packet! UDP protobuf message has required fields and can throw exception if the message is incomplete.
	delete[] recv_buffer;

	std::clog << "[" << getComponentName() << "] Local <- " << asio_endpoint->address().to_string() << ":"
			<< message.port() << std::endl;

	crypto::PublicKeyDSA pubkey = crypto::PublicKeyDSA::fromBinaryString(message.src_pubkey());

	try {
		if(!pubkey.validate())
			reject(messaging::Reason::KEY_INVALID, "Public key is inconsistent");
		if(!pubkey.verify(message.src_pubkey(), message.signature()))
			reject(messaging::Reason::KEY_INVALID, "Signature is invalid");
		/*
		 * Summary: We received a message, which contains TransportSocketEndpoint (well, we can create it from this message)
		 */

		peer::TH th = peer::TH::compute(message.src_pubkey());

		// Converting Boost::asio endpoint representation to string, so we could pass it as an argument to our network backend.
		transport::UDPTransportInterfaceEndpoint interface_endpoint(asio_endpoint->address().to_string(), message.port());
		transport::TransportSocketEndpoint socket_endpoint(std::make_shared<const UDPTransportInterfaceEndpoint>(interface_endpoint));
		messaging::PeerProcessor::getInstance()->processNewPeerConnection(th, socket_endpoint, pubkey);
	} catch(messaging::RejectException *e) {
		std::clog << e->what();
		delete e;
	}

	// We received message, continue receiving others
	receive();
}

messaging::protocol::UDPLPDMessage UDPLPD::generateLPDMessage() {
	databases::PersonalKeyStorage* pks = databases::PersonalKeyStorage::getInstance();

	messaging::protocol::UDPLPDMessage message;
	message.set_port(getUDPPort());

	message.set_src_pubkey(pks->getMyPublicKey().toBinaryString());

	// We need to sign this message for security reasons.
	std::string signature = pks->getMyPrivateKey().sign(pks->getMyPublicKey().toBinaryString());
	message.set_signature(signature);

	return message;
}

void UDPLPD::send() {
	std::clog << "[" << getComponentName() << "] Local -> " << m_target_address.to_string() << ":" << m_target_port
			<< std::endl;
	m_lpd_socket.async_send_to(buffer(generateLPDMessage().SerializeAsString()),
			ip::udp::endpoint(m_target_address, m_target_port), boost::bind(&UDPLPD::waitBeforeSend, this));
}

void UDPLPD::receive() {
	char* lpd_packet = new char[2048];
	std::shared_ptr< ip::udp::endpoint > endpoint = std::make_shared< ip::udp::endpoint >(m_bind_address,
			m_target_port);
	m_lpd_socket.async_receive_from(buffer(lpd_packet, 2048), *endpoint,
			boost::bind(&UDPLPD::processReceived, this, placeholders::bytes_transferred, endpoint, lpd_packet));
}

void UDPLPD::configChanged(){

}

void UDPLPD::startSend() {
	initSocket();
	std::clog << "[" << getComponentName() << "] Started sending broadcasts to: " << m_target_address << ":"
			<< m_target_port << std::endl;
	send();
}

void UDPLPD::startReceive() {
	initSocket();
	std::clog << "[" << getComponentName() << "] Started receiving broadcasts from: " << m_target_address << ":"
			<< m_target_port << std::endl;
	receive();
}

} /* namespace lpd */
} /* namespace net */
} /* namespace p2pnet */
