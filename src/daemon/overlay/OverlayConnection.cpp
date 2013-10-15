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
#include "OverlayConnection.h"
#include "../databases/PersonalKeyStorage.h"
#include <algorithm>

namespace p2pnet {
namespace overlay {

OverlayConnection::OverlayConnection(overlay::TH th) : th_endpoint(th) {
	log() << "New Overlay Connection initiated with TH:" << th_endpoint.toBase58() << std::endl;
}
OverlayConnection::~OverlayConnection() {}

void OverlayConnection::sendRaw(std::string data) {
	auto& transport_socket_connections = transport::TransportSocket::getInstance()->m_connections;

	std::shared_ptr<transport::TransportConnection> conn;

	// Searching for at least one "living" TransportConnection
	for(auto& conn_it : m_tse){
		auto sock_it = transport_socket_connections.find(conn_it);
		if(sock_it->second->connected()){
			conn = sock_it->second;
			break;
		}
	}

	if(!conn){
		suspended_connection_messages.push_front(data);
	}
	conn->send(data);
}

bool OverlayConnection::isReady() const {
	return !(m_tse.empty());
}

void OverlayConnection::send(std::string data) {
}

void OverlayConnection::process(std::string data, transport::TransportSocketEndpoint from) {
	protocol::OverlayMessageStructure message;
	message.ParseFromString(data);

	auto it = std::find(m_tse.begin(), m_tse.end(), from);
	if(it == m_tse.end()){
		m_tse.push_front(from);
	}else{
		m_tse.erase(it);	// TODO: some spoofing attack could be attempted here. For example, one packet from malicious ip could block any connection to this peer.
		m_tse.push_front(from);
	}

	auto pks = databases::PersonalKeyStorage::getInstance();
	overlay::TH dest_th = overlay::TH::fromBinaryString(message.header().dest_th());

	if(pks->getPrivateKeyOfTH(dest_th) == nullptr){
		// This message is completely stale, or it is intended to be retransmitted.
	}else{
		auto payload = message.payload();

		if(payload.message_type() == payload.CONNECTION_PUBKEY || payload.message_type() == payload.CONNECTION_ECDH || payload.message_type() == payload.CONNECTION_ACK){
			processConnectionMessage(message);
		}
	}
}

void OverlayConnection::processConnectionMessage(protocol::OverlayMessageStructure message) {
	auto payload = message.payload();
	if(payload.message_type() == payload.CONNECTION_PUBKEY){
		protocol::OverlayMessageStructure::Payload::ConnectionPart conn_part;
		if(conn_part.ParseFromString(payload.serialized_payload())){
			bool ack = conn_part.ack();	// It means, that this is an answer.

			auto message_dsa_pubkey = crypto::PublicKeyDSA::fromBinaryString(conn_part.src_ecdsa_pubkey());

			if(crypto::Hash(message_dsa_pubkey) == th_endpoint && message_dsa_pubkey.validate()){
				public_key = message_dsa_pubkey;
			}else{
				return;	// Drop.
			}

			if(!ack){	// We received PUBKEY message, so we need to send back PUBKEY_ACK.
				state = PUBKEY_RECEIVED;
			}else{
				// We received PUBKEY_ACK message, so we need to send back ECDH.
				state = ECDH_SENT;
			}
		}
	}
}

} /* namespace overlay */
} /* namespace p2pnet */
