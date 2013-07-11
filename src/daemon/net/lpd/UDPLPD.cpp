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
#include "../../peer/TH.h"
#include <ctime>

namespace p2pnet {
namespace net {
namespace lpd {

UDPLPD::UDPLPD(ConfigManager& parent_config) : GenericLPD(parent_config),
		m_io_service(AsioIOService::getIOService()), m_timer(m_io_service), lpd_socket(
				m_io_service) {}

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

		if(! databases::NetDBStorage::getInstance()->hasEntry(th)){
			std::clog << "[" << getComponentName() << "] Discovered peer: " << th.toBase58() << std::endl;
		}

		databases::NetDBEntry& peer_recv = databases::NetDBStorage::getInstance()->getEntry(th);

		// Converting Boost::asio endpoint representation to string, so we could pass it as an argument to our network backend.
		net::UDPTransportInterfaceEndpoint endpoint(asio_endpoint->address().to_string(), message.port());
		if(! databases::NetDBStorage::getInstance()->hasRouteToPeer(th, endpoint.toProtobuf()) ){
			databases::TimedTSE* timed_tse = peer_recv.mutable_tse_s()->Add();
			*(timed_tse->mutable_tse_s()) = endpoint.toProtobuf();
			timed_tse->set_last_usage((google::protobuf::int64)std::time(0));
		}else{
			/*
			 * This function should be called when we want to "bump" selected route,
			 * so it is set to preferred.
			 */
			databases::NetDBStorage::getInstance()->bumpRouteToPeer(th, endpoint.toProtobuf());
		}

		auto session_ptr = (*messaging::SessionStorage::getInstance())[th.toBinaryString()];
		session_ptr->sendConnectionMessage();
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
	message.set_port(this->getConfigValueOrDefault<unsigned short>("net.sockets.udp.port"));

	message.set_src_pubkey(pks->getMyPublicKey().toBinaryString());

	// We need to sign this message for security reasons.
	std::string signature = pks->getMyPrivateKey().sign(pks->getMyPublicKey().toBinaryString());
	message.set_signature(signature);

	return message;
}

void UDPLPD::send() {
	std::clog << "[" << getComponentName() << "] Local -> " << target_ipv4_multicast.address().to_string() << ":" << target_ipv4_multicast.port();
	lpd_socket.send_to(buffer(generateLPDMessage().SerializeAsString()), target_ipv4_multicast);
	lpd_socket.async_send_to(buffer(generateLPDMessage().SerializeAsString()), target_ipv6_multicast, boost::bind(&UDPLPD::waitBeforeSend, this));
}

void UDPLPD::receive() {
	char* lpd_packet4 = new char[2048];
	char* lpd_packet6 = new char[2048];
	std::shared_ptr< ip::udp::endpoint > endpoint4 = std::make_shared< ip::udp::endpoint >(target_ipv4_multicast);
	std::shared_ptr< ip::udp::endpoint > endpoint6 = std::make_shared< ip::udp::endpoint >(target_ipv6_multicast);
	lpd_socket.async_receive_from(buffer(lpd_packet4, 2048), *endpoint4,
			boost::bind(&UDPLPD::processReceived, this, placeholders::bytes_transferred, endpoint4, lpd_packet4));
	lpd_socket.async_receive_from(buffer(lpd_packet6, 2048), *endpoint6,
			boost::bind(&UDPLPD::processReceived, this, placeholders::bytes_transferred, endpoint6, lpd_packet6));
}

void UDPLPD::startSend() {
	std::clog << "[" << getComponentName() << "] Started sending broadcasts to:" << std::endl <<
			"[" << getComponentName() << "]" << "   IPv4: " << target_ipv4_multicast.address().to_string() << ":" << target_ipv4_multicast.port() << std::endl <<
			"[" << getComponentName() << "]" << "   IPv6: " << target_ipv6_multicast.address().to_string() << ":" << target_ipv6_multicast.port() << std::endl;
	send();
}

void UDPLPD::initSocket() {
	lpd_socket.open(ip::udp::v6());

	lpd_socket.set_option(ip::multicast::join_group(m_target_address));
	lpd_socket.set_option(ip::multicast::enable_loopback(true));
	lpd_socket.set_option(ip::udp::socket::reuse_address(true));
	lpd_socket.set_option(ip::v6_only(true));

	lpd_socket.bind(local_multicast);
}

void UDPLPD::readConfig() {
	this->m_timer_seconds = getConfig().get("net.lpd.udp.timer", getDefaults().get<unsigned int>("net.lpd.udp.timer"));
	this->target_ipv4_multicast = ip::udp::endpoint(
			ip::address::from_string(this->getConfigValueOrDefault<std::string>("net.lpd.udp.mcast.host_v4")),
			this->getConfigValueOrDefault<unsigned short>("net.lpd.udp.mcast.port")
	);
	this->target_ipv6_multicast = ip::udp::endpoint(
			ip::address::from_string(this->getConfigValueOrDefault<std::string>("net.lpd.udp.mcast.host_v6")),
			this->getConfigValueOrDefault<unsigned short>("net.lpd.udp.mcast.port")
	);
	this->local_multicast = ip::udp::endpoint(
			ip::address::from_string(this->getConfigValueOrDefault<std::string>("net.lpd.udp.local_ip")),
			this->getConfigValueOrDefault<unsigned short>("net.lpd.udp.mcast.port")
	);


}

void UDPLPD::configChanged() {
}

void UDPLPD::startReceive() {
	std::clog << "[" << getComponentName() << "] Started receiving broadcasts from:" << std::endl <<
			"[" << getComponentName() << "]" << "   IPv4: " << target_ipv4_multicast.address().to_string() << ":" << target_ipv4_multicast.port() << std::endl <<
			"[" << getComponentName() << "]" << "   IPv6: " << target_ipv6_multicast.address().to_string() << ":" << target_ipv6_multicast.port() << std::endl;
	receive();
}

} /* namespace lpd */
} /* namespace net */
} /* namespace p2pnet */
