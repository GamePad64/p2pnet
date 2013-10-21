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
#include "../AsioIOService.h"
#include <algorithm>

namespace p2pnet {
namespace overlay {

OverlayConnection::OverlayConnection(overlay::TH th) : th_endpoint(th), udp_key_rotation_limit(AsioIOService::getIOService()) {
	pks = databases::PersonalKeyStorage::getInstance();
	log() << "New Overlay Connection initiated with TH:" << th_endpoint.toBase58() << std::endl;
}
OverlayConnection::~OverlayConnection() {}

void OverlayConnection::sendData(std::string data) {
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

void OverlayConnection::sendMessage(const protocol::OverlayMessage& send_message) {
	auto new_send_message = send_message;

	auto our_hist_key = pks->getPrivateKeyOfTH(crypto::Hash::fromBinaryString(send_message.header().src_th()));	//TODO: Alarm! Possible race condition, as our keys update in a separate thread.
	if(our_hist_key != nullptr && our_hist_key->toBinaryString() != pks->getMyPrivateKey().toBinaryString()){
		if(send_message.header().prio() == send_message.header().RELIABLE){
			if(!new_send_message.mutable_payload()->has_key_rotation_part()){
				new_send_message.mutable_payload()->mutable_key_rotation_part()->CopyFrom(generateKeyRotationPart(send_message, our_hist_key));
			}
		}else if(send_message.header().prio() == send_message.header().REALTIME){
			if(!udp_key_rotation_locked){
				udp_key_rotation_limit.expires_from_now(boost::posix_time::seconds(KEY_ROTATION_LIMIT));
				protocol::OverlayMessage key_rotation_message;
				key_rotation_message.mutable_header()->CopyFrom(send_message.header());
				key_rotation_message.mutable_payload()->mutable_connection_part()->CopyFrom(generateKeyRotationPart(send_message, our_hist_key));
				udp_key_rotation_locked = true;
				udp_key_rotation_limit.async_wait(boost::bind([&](){udp_key_rotation_locked = false;}, this));
			}
		}
	}
}

bool OverlayConnection::performLocalKeyRotation(const protocol::OverlayMessage& recv_message){
	auto& part = recv_message.payload().key_rotation_part();

	if(bool(public_key)){
		auto oldkey = crypto::PublicKeyDSA::fromBinaryString(part.old_ecdsa_key());
		auto newkey = crypto::PublicKeyDSA::fromBinaryString(part.new_ecdsa_key());

		if(overlay::TH(oldkey) == th_endpoint){	// Okay, this message is not alien. Checking.
			auto twokeys = part.old_ecdsa_key()+part.new_ecdsa_key();

			if(oldkey.verify(twokeys, part.old_signature()) && newkey.validate() && newkey.verify(twokeys, part.new_signature())){
				//This message is genuine. We are changing public key, TH, and connection in socket's map.
				public_key = newkey;
				th_endpoint = overlay::TH(public_key);
				OverlaySocket::getInstance()->m_connections.insert(std::make_pair(th_endpoint, shared_from_this()));
				return true;
			}
		}
	}//Else, we can't check these signatures, so we drop this message.
	return false;
}

protocol::OverlayMessage OverlayConnection::generateReplySkel(const protocol::OverlayMessage& recv_message){
	protocol::OverlayMessage new_message;

	new_message.mutable_header()->set_src_th(recv_message.header().dest_th());
	new_message.mutable_header()->set_dest_th(recv_message.header().src_th());

	return new_message;
}

protocol::OverlayMessage_Payload_ConnectionPart OverlayConnection::generateConnectionPart(protocol::OverlayMessage_Payload_ConnectionPart_ConnectionStage for_stage){
	protocol::OverlayMessage_Payload_ConnectionPart conn_part;

	/* PUBKEY part */
	if(for_stage == protocol::OverlayMessage_Payload_ConnectionPart_ConnectionStage_PUBKEY
			|| for_stage == protocol::OverlayMessage_Payload_ConnectionPart_ConnectionStage_PUBKEY_ACK){
		conn_part.set_stage(for_stage);
		conn_part.set_src_ecdsa_pubkey(pks->getMyPublicKey().toBinaryString());
	}

	/* ECDH part */
	else if(for_stage == protocol::OverlayMessage_Payload_ConnectionPart_ConnectionStage_ECDH
			|| for_stage == protocol::OverlayMessage_Payload_ConnectionPart_ConnectionStage_ECDH_ACK){
		conn_part.set_stage(for_stage);

		if(!bool(ecdh_key))
			ecdh_key.generateKey();

		auto public_ecsdh_component = ecdh_key.derivePublicKey();

		conn_part.set_src_ecdh_pubkey(public_ecsdh_component);
		conn_part.set_signature(pks->getMyPrivateKey().sign(public_ecsdh_component));
	}

	/* AES part */
	else if(for_stage == protocol::OverlayMessage_Payload_ConnectionPart_ConnectionStage_AES){
		conn_part.set_stage(for_stage);

		if(!bool(ecdh_key))
			ecdh_key.generateKey();

		auto public_ecsdh_component = ecdh_key.derivePublicKey();

		conn_part.set_src_ecdh_pubkey(public_ecsdh_component);
		conn_part.set_signature(pks->getMyPrivateKey().sign(public_ecsdh_component));
	}

	return conn_part;
}

protocol::OverlayMessage_Payload_KeyRotationPart OverlayConnection::generateKeyRotationPart(const protocol::OverlayMessage& send_message, std::shared_ptr<crypto::PrivateKeyDSA> our_hist_key){
	protocol::OverlayMessage_Payload_KeyRotationPart part;

	std::string old_key_s = our_hist_key->derivePublicKey().toBinaryString();
	std::string new_key_s = pks->getMyPublicKey().toBinaryString();

	part.set_old_ecdsa_key(old_key_s);
	part.set_new_ecdsa_key(new_key_s);

	part.set_old_signature(our_hist_key->sign(old_key_s+new_key_s));
	part.set_new_signature(pks->getMyPrivateKey().sign(old_key_s+new_key_s));

	return part;
}

void OverlayConnection::processTransmissionControlPart(protocol::OverlayMessageStructure& answ_message,
		bool& send_answ,
		const protocol::OverlayMessageStructure& recv_message,
		const protocol::OverlayMessageStructure_Payload_Part& part){
	if(part.payload_type() == part.TRANSMISSION_CONTROL
			&& answ_message.header().prio() == answ_message.header().RELIABLE){
		protocol::OverlayMessageStructure_Payload_Part_TransmissionControlPart uns_tcpart;
		uns_tcpart.ParseFromString(part.serialized_part());

		auto payload_ptr = answ_message.mutable_payload()->add_payload_parts();
		payload_ptr->set_payload_type(payload_ptr->TRANSMISSION_CONTROL);

		protocol::OverlayMessageStructure_Payload_Part_TransmissionControlPart part_to_serialize;

		part_to_serialize.set_seq_num(seq_counter++);
		*(part_to_serialize.mutable_ack_num()->Add()) = uns_tcpart.seq_num();
		payload_ptr->set_serialized_part(part_to_serialize.SerializeAsString());
	}
}

bool OverlayConnection::isReady() const {
	return state == ESTABLISHED;
}

void OverlayConnection::updateTSE(const transport::TransportSocketEndpoint& from, bool verified) {
	auto it = std::find(m_tse.begin(), m_tse.end(), from);
	if(it == m_tse.end()){
		m_tse.push_front(from);
	}else{
		m_tse.erase(it);
		if(verified){
			m_tse.push_front(from);
		}else{
			m_tse.push_back(from);
		}
	}
}

void OverlayConnection::send(const protocol::OverlayMessage& send_message) {
}

void OverlayConnection::process(const protocol::OverlayMessage& recv_message, const transport::TransportSocketEndpoint& from) {
	updateTSE(from);

	std::shared_ptr<crypto::PrivateKeyDSA> our_historic_ecdsa_privkey;
	overlay::TH dest_th = overlay::TH::fromBinaryString(recv_message.header().dest_th());
	our_historic_ecdsa_privkey = pks->getPrivateKeyOfTH(dest_th);

	if(our_historic_ecdsa_privkey != nullptr){
		// So, this message is for us.
		if(recv_message.payload().has_key_rotation_part())
			if(performLocalKeyRotation(recv_message))
				updateTSE(from, true);
		processConnectionPart(reply_message, send_answer, recv_omessage, payload_part);
	}else{
		// This message is completely stale, or it is intended to be retransmitted.
	}
}

void OverlayConnection::processConnectionPart(protocol::OverlayMessageStructure& answ_message,
		bool& send_answ,
		const protocol::OverlayMessageStructure& recv_message,
		const protocol::OverlayMessageStructure_Payload_Part& part) {
	if(part.payload_type() == part.CONNECTION_PUBKEY){
		processConnectionPartPUBKEY(answ_message, send_answ, recv_message, part);
	}else if(part.payload_type() == part.CONNECTION_ECDH){
		processConnectionPartECDH(answ_message, send_answ, recv_message, part);
	}else if(part.payload_type() == part.CONNECTION_ACK){
		processConnectionPartACK(answ_message, send_answ, recv_message, part);
	}
}

void OverlayConnection::processConnectionPartPUBKEY(protocol::OverlayMessageStructure& answ_message,
		bool& send_answ,
		const protocol::OverlayMessageStructure& recv_message,
		const protocol::OverlayMessageStructure_Payload_Part& part){
	protocol::OverlayMessageStructure::Payload::Part::ConnectionPart conn_part;
	if(!conn_part.ParseFromString(part.serialized_part())){
		return;	// TODO: On parse error we just drop packet now. TODO MessageReject.
	}

	auto recv_dsa_pubkey = crypto::PublicKeyDSA::fromBinaryString(conn_part.src_ecdsa_pubkey());

	if((crypto::Hash(recv_dsa_pubkey) == th_endpoint)	// So, we check if this message contains genuine ECDSA public key of connected TH.
			&& recv_dsa_pubkey.validate()){	// And then we validate this ECDSA public key using mathematical methods.
		log() << "Received public key from: TH:" << th_endpoint.toBase58() << std::endl;
		public_key = recv_dsa_pubkey;
	}else{
		return;	// Drop. TODO MessageReject.
	}

	protocol::OverlayMessageStructure::Payload::Part* new_part_ptr = answ_message.mutable_payload()->add_payload_parts();
	send_answ = true;

	if(!conn_part.ack()){	// We received PUBKEY message, so we need to send back PUBKEY_ACK.
		new_part_ptr->set_payload_type(new_part_ptr->CONNECTION_PUBKEY);
		new_part_ptr->set_serialized_part(generateConnectionPartPUBKEY(true).SerializeAsString());

		state = PUBKEY_RECEIVED;
	}else{	// We received PUBKEY_ACK message, so we need to send back ECDH.
		new_part_ptr->set_payload_type(new_part_ptr->CONNECTION_ECDH);
		new_part_ptr->set_serialized_part(generateConnectionPartECDH(false).SerializeAsString());

		state = ECDH_SENT;
	}
}

void OverlayConnection::processConnectionPartECDH(protocol::OverlayMessageStructure& answ_message,
		bool& send_answ,
		const protocol::OverlayMessageStructure& recv_message,
		const protocol::OverlayMessageStructure_Payload_Part& part){
}

void OverlayConnection::processConnectionPartACK(protocol::OverlayMessageStructure& answ_message,
		bool& send_answ,
		const protocol::OverlayMessageStructure& recv_message,
		const protocol::OverlayMessageStructure_Payload_Part& part){
}

/*void OverlayConnection::processConnectionECDHMessage(protocol::OverlayMessageStructure message){
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
}*/


} /* namespace overlay */
} /* namespace p2pnet */
