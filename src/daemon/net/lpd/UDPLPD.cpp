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
#include "../udp/UDPTransportSocket.h"
#include "../../protobuf/Protocol.pb.h"
#include "../../databases/PersonalKeyStorage.h"
#include "../../messaging/MessageGenerator.h"
#include "../../peer/TH.h"

namespace p2pnet {
namespace net {
namespace lpd {

UDPLPD::UDPLPD(Config& config, net::UDPTransportSocket& udp_socket, databases::NetDBStorage& netdb_storage) :
		m_config(config), m_io_service(AsioIOService::getIOService()), m_timer(m_io_service), m_lpd_socket(
				m_io_service), m_udp_socket(udp_socket), m_netdb_storage(netdb_storage) {
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

void UDPLPD::processReceived(size_t bytes, std::shared_ptr< ip::udp::endpoint > asio_endpoint, char* recv_buffer) {
	// We received message, continue receiving others
	receive();

	// Create Protocol Buffer message
	messaging::protocol::UDPLPDMessage message;
	message.ParseFromString(std::string(recv_buffer, bytes));
	// EXCEPTION: Corrupted packet! UDP protobuf message has required fields and can throw exception if the message is incomplete.

	std::clog << "[" << getServiceName() << "] Local <- " << asio_endpoint->address().to_string() << ":"
			<< message.port() << std::endl;

	if (checkLPDMessage(message)) {
		// Converting Boost::asio endpoint representation to string, so we could pass it as an argument to our network backend.
		std::string received_address = asio_endpoint->address().to_string();
		net::UDPTransportSocketEndpoint endpoint(received_address, message.port());

		peer::TH th = peer::TH::compute(message.src_pubkey());

		if(! m_netdb_storage.hasPeer(th)){
			std::clog << "[" << getServiceName() << "] Discovered peer: " << th.toBase58() << std::endl;
		}

		peer::Peer& peer_recv = m_netdb_storage.getPeer(th);

		if(! peer_recv.hasRoute(endpoint) ){
			peer_recv.addRoute(endpoint);
		}//else{
			/*
			 * This function should be called when we want to "bump" selected route,
			 * so it is set to preferred.
			 */
		//	peer_recv.bumpRoute(endpoint);
		//}

		sendKeyExchangeMessage(endpoint, th);
	} else {
		// This packet is fake (or, maybe, corrupted)
		std::clog << "[" << getServiceName() << "] Packet rejected." << std::endl;
	}

	delete[] recv_buffer;
}

void UDPLPD::sendKeyExchangeMessage(net::UDPTransportSocketEndpoint& endpoint, const peer::TH& dest_th){
	messaging::MessageGenerator generator;
	auto message = generator.generateMessage(dest_th, generator.generateKeyExchangePayload());

	m_udp_socket.hereSendTo(endpoint, message.SerializeAsString());
	std::clog << "[" << getServiceName() << "] Sent agreement request to " << endpoint.getIP() << std::endl;
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

bool UDPLPD::checkLPDMessage(const messaging::protocol::UDPLPDMessage& message){
	crypto::PublicKeyDSA pubkey = crypto::PublicKeyDSA::fromBinaryString(message.src_pubkey());

	return pubkey.validate() && pubkey.verify(message.src_pubkey(), message.signature());
}

void UDPLPD::send() {
	std::clog << "[" << getServiceName() << "] Local -> " << m_target_address.to_string() << ":" << m_target_port
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

void UDPLPD::startSend() {
	std::clog << "[" << getServiceName() << "] Started sending broadcasts to: " << m_target_address << ":"
			<< m_target_port << std::endl;
	send();
}

void UDPLPD::startReceive() {
	std::clog << "[" << getServiceName() << "] Started receiving broadcasts from: " << m_target_address << ":"
			<< m_target_port << std::endl;
	receive();
}

} /* namespace lpd */
} /* namespace net */
} /* namespace p2pnet */
