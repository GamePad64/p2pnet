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
#include "../transport/TransportSocket.h"
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

	if(conn){
		conn->send(data);
	}else{
		// If there are no connections alive, we store messages to "suspended".
		// And, when they will be sent after some connections arrive.
		suspended_messages.push_front(data);
	}
}

bool OverlayConnection::isReady() const {
	return !(m_tse.empty());
}

void OverlayConnection::addTransportSocketEndpoint(transport::TransportSocketEndpoint from) {
	auto it = std::find(m_tse.begin(), m_tse.end(), from);
	if(it == m_tse.end()){
		m_tse.push_front(from);
	}else{
		m_tse.erase(it);	// TODO: some spoofing attack could be attempted here. For example, one packet from malicious ip could block any connection to this peer.
		m_tse.push_front(from);
	}
}

void OverlayConnection::send(std::string data) {
}

void OverlayConnection::process(std::string data, transport::TransportSocketEndpoint from) {
	protocol::OverlayMessageStructure message;
	message.ParseFromString(data);

	addTransportSocketEndpoint(from);

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
		processConnectionPubkeyMessage(message);
	}
}

void OverlayConnection::processConnectionPubkeyMessage(protocol::OverlayMessageStructure message){
	auto payload = message.payload();
	protocol::OverlayMessageStructure::Payload::ConnectionPart conn_part;
	if(conn_part.ParseFromString(payload.serialized_payload())){
		bool ack = conn_part.ack();	// It means, that this is an answer.

		auto message_dsa_pubkey = crypto::PublicKeyDSA::fromBinaryString(conn_part.src_ecdsa_pubkey());

		if((crypto::Hash(message_dsa_pubkey) == th_endpoint) && message_dsa_pubkey.validate()){
			log() << "Received public key from: TH:" << th_endpoint.toBase58() << std::endl;
			public_key = message_dsa_pubkey;
		}else{
			return;	// Drop.
		}

		// Then we generate new message.
		protocol::OverlayMessageStructure new_message;
		new_message.mutable_header()->set_src_th(
				databases::PersonalKeyStorage::getInstance()->getMyTransportHash().toBinaryString());
		new_message.mutable_header()->set_dest_th(th_endpoint.toBinaryString());

		protocol::OverlayMessageStructure::Payload::ConnectionPart new_conn_part;
		if(!ack){	// We received PUBKEY message, so we need to send back PUBKEY_ACK.
			new_message.mutable_payload()->set_message_type(new_message.payload().CONNECTION_PUBKEY);
			new_conn_part.set_ack(true);
			new_conn_part.set_src_ecdsa_pubkey(databases::PersonalKeyStorage::getInstance()->getMyPublicKey().toBinaryString());
			state = PUBKEY_RECEIVED;
		}else{
			new_message.mutable_payload()->set_message_type(new_message.payload().CONNECTION_ECDH);
			// We received PUBKEY_ACK message, so we need to send back ECDH.
			ecdh_key.generateKey();
			std::string ecdh_public = ecdh_key.derivePublicKey();
			new_conn_part.set_src_ecdh_pubkey(ecdh_public);

			std::string signature = databases::PersonalKeyStorage::getInstance()->getMyPrivateKey().sign(ecdh_public+th_endpoint.toBinaryString());
			new_conn_part.set_signature(signature);

			state = ECDH_SENT;
		}
		new_message.mutable_payload()->set_serialized_payload(new_conn_part.SerializeAsString());
		sendRaw(new_message.SerializeAsString());
	}
}

} /* namespace overlay */
} /* namespace p2pnet */
