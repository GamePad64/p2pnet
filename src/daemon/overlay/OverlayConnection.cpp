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
#include <functional>
#include <boost/date_time.hpp>

namespace p2pnet {
namespace overlay {

OverlayConnection::OverlayConnection(std::shared_ptr<OverlayPeer> overlay_peer) : overlay_peer_ptr(overlay_peer),
		udp_key_rotation_limit(AsioIOService::getIOService()) {
	log() << "New Overlay Connection initiated with TH:" << overlay_peer_ptr->getPeerTH().toBase58() << std::endl;
}
OverlayConnection::~OverlayConnection() {
	disconnect();
}

void OverlayConnection::keysUpdated(){
	if(state == ESTABLISHED){
		protocol::OverlayMessage message;
		message.mutable_header()->set_src_th((getHistory().begin()+1)->second->toBinaryString());
		message.mutable_header()->set_dest_th(overlay_peer_ptr->getPeerTH().toBinaryString());
		message.mutable_header()->set_prio(message.header().REALTIME);

		message.mutable_payload()->mutable_key_rotation_part()->CopyFrom(generateKeyRotationPart(*((getHistory().begin()+1)->first)));
		sendMessage(message);
	}
}

void OverlayConnection::setState(const States& state_to_set){
	state = state_to_set;
	if(state_to_set == ESTABLISHED && !suspended_payloads.empty()){
		for(auto& payload : suspended_payloads){
			send(payload.first, payload.second);
		}
	}
}

void OverlayConnection::sendData(std::string data) {
	auto& transport_socket_connections = transport::TransportSocket::getInstance()->m_connections;

	std::shared_ptr<transport::TransportConnection> conn;

	// Searching for at least one "living" TransportConnection
	for(auto& conn_it : overlay_peer_ptr->getEndpointList()){
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
		suspended_data.push_front(data);
	}
}

void OverlayConnection::sendMessage(const protocol::OverlayMessage& send_message) {
	auto new_send_message = send_message;

	auto our_hist_key = getPrivateKeyOfTH(crypto::Hash::fromBinaryString(send_message.header().src_th()));	//TODO: Alarm! Possible race condition, as our keys update in a separate thread.
	if(our_hist_key != nullptr && our_hist_key->toBinaryString() != getMyPrivateKey().toBinaryString()){
		if(send_message.header().prio() == send_message.header().RELIABLE){
			if(!new_send_message.mutable_payload()->has_key_rotation_part()){
				new_send_message.mutable_payload()->mutable_key_rotation_part()->CopyFrom(generateKeyRotationPart(*our_hist_key));
			}
		}else if(send_message.header().prio() == send_message.header().REALTIME && !new_send_message.mutable_payload()->has_key_rotation_part()){
			if(!udp_key_rotation_locked){
				udp_key_rotation_limit.expires_from_now(boost::posix_time::seconds(KEY_ROTATION_LIMIT));
				protocol::OverlayMessage key_rotation_message;
				key_rotation_message.mutable_header()->CopyFrom(send_message.header());
				key_rotation_message.mutable_payload()->mutable_connection_part()->CopyFrom(generateKeyRotationPart(*our_hist_key));
				sendMessage(key_rotation_message);
				udp_key_rotation_locked = true;
				udp_key_rotation_limit.async_wait([&](const boost::system::error_code& ec) {if (!ec) udp_key_rotation_locked = false;});
			}
		}
	}

	if(send_message.header().prio() == send_message.header().RELIABLE){
		new_send_message.mutable_header()->set_seq_num(seq_counter++);
		for(auto& it : acked_messages){
			new_send_message.mutable_header()->mutable_ack_num()->Add(it);
		}
	}

	log() << "Message sent to: TH:" << overlay_peer_ptr->getPeerTH().toBase58() << std::endl;

	sendData(new_send_message.SerializeAsString());
}

bool OverlayConnection::performLocalKeyRotation(const protocol::OverlayMessage& recv_message){
	auto& part = recv_message.payload().key_rotation_part();

	if(overlay_peer_ptr->getPublicKey().isPresent()){
		auto oldkey = crypto::PublicKeyDSA::fromBinaryString(part.old_ecdsa_key());
		auto newkey = crypto::PublicKeyDSA::fromBinaryString(part.new_ecdsa_key());

		if(overlay::TH(oldkey) == overlay_peer_ptr->getPeerTH()){	// Okay, this message is not alien. Checking.
			auto twokeys = part.old_ecdsa_key()+part.new_ecdsa_key();

			if(oldkey.verify(twokeys, part.old_signature()) && newkey.validate() && newkey.verify(twokeys, part.new_signature())){
				//This message is genuine. We are changing public key. TH, connection in socket's map, position in K-buckets will be changed automatically.
				overlay_peer_ptr->setPublicKey(newkey);
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
		conn_part.set_src_ecdsa_pubkey(getMyPublicKey().toBinaryString());
		conn_part.set_expires(boost::posix_time::to_iso_string(getExpiryTime()));
		conn_part.set_lost(boost::posix_time::to_iso_string(getExpiryTime()));
	}

	/* ECDH part */
	else if(for_stage == protocol::OverlayMessage_Payload_ConnectionPart_ConnectionStage_ECDH
			|| for_stage == protocol::OverlayMessage_Payload_ConnectionPart_ConnectionStage_ECDH_ACK){
		conn_part.set_stage(for_stage);

		if(!overlay_peer_ptr->getECDHKey().isPresent())
			overlay_peer_ptr->setECDHKey(crypto::ECDH::generateNewKey());

		auto public_ecdh_component = overlay_peer_ptr->getECDHKey().derivePublicKey();

		conn_part.set_src_ecdh_pubkey(public_ecdh_component);
		conn_part.set_signature(getMyPrivateKey().sign(public_ecdh_component));
	}

	/* AES part */
	else if(for_stage == protocol::OverlayMessage_Payload_ConnectionPart_ConnectionStage_AES){
		conn_part.set_stage(for_stage);	// Consists of stage only
	}

	return conn_part;
}

protocol::OverlayMessage_Payload_KeyRotationPart OverlayConnection::generateKeyRotationPart(const crypto::PrivateKeyDSA& our_hist_key){
	protocol::OverlayMessage_Payload_KeyRotationPart part;

	std::string old_key_s = our_hist_key.derivePublicKey().toBinaryString();
	std::string new_key_s = getMyPublicKey().toBinaryString();

	part.set_old_ecdsa_key(old_key_s);
	part.set_new_ecdsa_key(new_key_s);

	part.set_old_signature(our_hist_key.sign(old_key_s+new_key_s));
	part.set_new_signature(getMyPrivateKey().sign(old_key_s+new_key_s));

	return part;
}

void OverlayConnection::processConnectionPart(const protocol::OverlayMessage& recv_message,
		const protocol::OverlayMessage_Payload& decrypted_payload) {
	if(recv_message.payload().has_connection_part()){
		auto& part = recv_message.payload().connection_part();
		if(part.stage() == part.PUBKEY || part.stage() == part.PUBKEY_ACK){
			processConnectionPartPUBKEY(recv_message);
		}else if(part.stage() == part.ECDH || part.stage() == part.ECDH_ACK){
			processConnectionPartECDH(recv_message);
		}
	}else if(decrypted_payload.has_connection_part()){
		auto& part = decrypted_payload.connection_part();
		if(part.stage() == part.AES){
			processConnectionPartAES(recv_message, decrypted_payload);
		}
	}
}

void OverlayConnection::processConnectionPartPUBKEY(const protocol::OverlayMessage& recv_message){
	auto recv_dsa_pubkey = crypto::PublicKeyDSA::fromBinaryString(recv_message.payload().connection_part().src_ecdsa_pubkey());

	if((crypto::Hash(recv_dsa_pubkey) == overlay_peer_ptr->getPeerTH())	// So, we check if this message contains genuine ECDSA public key of connected TH.
			&& recv_dsa_pubkey.validate()){	// And then we validate this ECDSA public key using mathematical methods.
		log() << "Received ECDSA public key from: TH:" << overlay_peer_ptr->getPeerTH().toBase58() << std::endl;
		overlay_peer_ptr->setPublicKey(recv_dsa_pubkey);

		overlay_peer_ptr->updateExpiryTime(boost::posix_time::from_iso_string(recv_message.payload().connection_part().expires()));
		overlay_peer_ptr->updateLostTime(boost::posix_time::from_iso_string(recv_message.payload().connection_part().lost()));
	}else{
		return;	// Drop. TODO MessageReject.
	}

	auto reply = generateReplySkel(recv_message);

	if(recv_message.payload().connection_part().stage() == recv_message.payload().connection_part().PUBKEY){
		/* We need to send back PUBKEY_ACK */
		reply.mutable_header()->set_prio(reply.header().RELIABLE);
		reply.mutable_payload()->mutable_connection_part()->CopyFrom(generateConnectionPart(reply.payload().connection_part().PUBKEY_ACK));

		setState(PUBKEY_RECEIVED);

		sendMessage(reply);
	}else if(recv_message.payload().connection_part().stage() == recv_message.payload().connection_part().PUBKEY_ACK){
		/* We need to send back ECDH */
		reply.mutable_header()->set_prio(reply.header().RELIABLE);
		reply.mutable_payload()->mutable_connection_part()->CopyFrom(generateConnectionPart(reply.payload().connection_part().ECDH));

		setState(ECDH_SENT);

		sendMessage(reply);
	}
}

void OverlayConnection::processConnectionPartECDH(const protocol::OverlayMessage& recv_message){
	if(!overlay_peer_ptr->getECDHKey().isPresent())
		overlay_peer_ptr->setECDHKey(crypto::ECDH::generateNewKey());
	auto salt_v = overlay_peer_ptr->getPeerTH() ^ getMyTransportHash();
	auto derived_aes_string = overlay_peer_ptr->getECDHKey().deriveSymmetricKey(crypto::AES::vectorSize(),
			recv_message.payload().connection_part().src_ecdh_pubkey(), std::string(salt_v.begin(), salt_v.end())
			);
	overlay_peer_ptr->setAESKey(crypto::AES::fromBinaryString(derived_aes_string));

	log() << "Received ECDH public key from: TH:" << overlay_peer_ptr->getPeerTH().toBase58() << std::endl;

	auto reply = generateReplySkel(recv_message);

	if(recv_message.payload().connection_part().stage() == recv_message.payload().connection_part().ECDH && state == PUBKEY_RECEIVED){
		/* We need to send back ECDH_ACK */
		reply.mutable_header()->set_prio(reply.header().RELIABLE);
		reply.mutable_payload()->mutable_connection_part()->CopyFrom(generateConnectionPart(reply.payload().connection_part().ECDH_ACK));

		setState(ECDH_RECEIVED);

		sendMessage(reply);
	}else if(recv_message.payload().connection_part().stage() == recv_message.payload().connection_part().ECDH_ACK && state == ECDH_SENT){
		/* We need to send back AES */
		reply.mutable_header()->set_prio(reply.header().RELIABLE);
		protocol::OverlayMessage_Payload not_encrypted_payload;
		not_encrypted_payload.mutable_connection_part()->CopyFrom(generateConnectionPart(reply.payload().connection_part().AES));
		reply.set_encrypted_payload(encryptPayload(not_encrypted_payload));

		sendMessage(reply);

		setState(ESTABLISHED);
		log() << "AES encrypted connection with TH:" << overlay_peer_ptr->getPeerTH().toBase58() << " established" << std::endl;
	}
}

void OverlayConnection::processConnectionPartAES(const protocol::OverlayMessage& recv_message,
		const protocol::OverlayMessage_Payload& decrypted_payload){
	setState(ESTABLISHED);
	log() << "AES encrypted connection with TH:" << overlay_peer_ptr->getPeerTH().toBase58() << " established" << std::endl;
}

bool OverlayConnection::isReady() const {
	return state == ESTABLISHED;
}

void OverlayConnection::updateTSE(const transport::TransportSocketEndpoint& from, bool verified) {
	auto it = std::find(overlay_peer_ptr->getEndpointList().begin(), overlay_peer_ptr->getEndpointList().end(), from);
	if(it == overlay_peer_ptr->getEndpointList().end()){
		overlay_peer_ptr->getEndpointList().push_front(from);
	}else{
		overlay_peer_ptr->getEndpointList().erase(it);
		if(verified){
			overlay_peer_ptr->getEndpointList().push_front(from);
		}else{
			overlay_peer_ptr->getEndpointList().push_back(from);
		}
	}
}

std::string OverlayConnection::encryptPayload(const protocol::OverlayMessage_Payload& payload){
	return overlay_peer_ptr->getAESKey().encrypt(payload.SerializeAsString());
}

void OverlayConnection::send(const protocol::OverlayMessage_Payload& send_payload, const protocol::OverlayMessage_Header_MessagePriority& prio) {
	if(state == ESTABLISHED){
		protocol::OverlayMessage message;
		message.mutable_header()->set_src_th(pks->getMyTransportHash().toBinaryString());
		message.mutable_header()->set_dest_th(overlay_peer_ptr->getPeerTH().toBinaryString());
		message.mutable_header()->set_prio(prio);
		message.set_encrypted_payload(encryptPayload(send_payload));
		sendMessage(message);
	}else{
		if(prio == protocol::OverlayMessage_Header_MessagePriority_RELIABLE){
			suspended_payloads.push_back(std::make_pair(send_payload, prio));
		}else if(prio == protocol::OverlayMessage_Header_MessagePriority_REALTIME){
			// TODO: We need a new queue for use with realtime messages.
		}
	}
}

void OverlayConnection::process(const protocol::OverlayMessage& recv_message, const transport::TransportSocketEndpoint& from) {
	updateTSE(from);

	std::shared_ptr<crypto::PrivateKeyDSA> our_historic_ecdsa_privkey;

	overlay::TH src_th = overlay::TH::fromBinaryString(recv_message.header().src_th());
	overlay::TH dest_th = overlay::TH::fromBinaryString(recv_message.header().dest_th());

	log() << "Received OverlayMessage from: TH:" << src_th.toBase58() << std::endl;

	our_historic_ecdsa_privkey = getPrivateKeyOfTH(dest_th);
	if(our_historic_ecdsa_privkey != nullptr){
		// So, this message is for us.
		/* Key Rotation stage */
		if(recv_message.payload().has_key_rotation_part())
			if(performLocalKeyRotation(recv_message))
				updateTSE(from, true);
		/* Encryption stage */
		if(recv_message.has_encrypted_payload()){
			protocol::OverlayMessage_Payload decrypted_payload;
			if(overlay_peer_ptr->getAESKey().isPresent()){
				auto decrypted_payload_s = overlay_peer_ptr->getAESKey().decrypt(recv_message.encrypted_payload());
				if(decrypted_payload.ParseFromString(decrypted_payload_s)){
					/* Encrypted processing */
					updateTSE(from, true);
					processConnectionPart(recv_message, decrypted_payload);
				}else{
					// Reconnect
				}
			}else{
				// Umm... Dont' know. TODO: reconnect.
			}
		}else{
			/* Open (unencrypted) processing */
			processConnectionPart(recv_message);
		}
		if(recv_message.header().prio() == recv_message.header().RELIABLE){
			processed_messages.insert(recv_message.header().seq_num());
			if(processed_messages.size() > getValue<size_t>("overlay.connection.processed_queue_size")){
				processed_messages.erase(processed_messages.begin());
			}
			acked_messages.insert(recv_message.header().seq_num());
			for(auto& it : recv_message.header().ack_num()){
				sent_message_buffer.erase(it);
			}
		}
	}else{
		// This message is completely stale, or it is intended to be retransmitted.
	}
}

void OverlayConnection::process(const protocol::ConnectionRequestMessage& recv_message,
		const transport::TransportSocketEndpoint& from) {
	updateTSE(from);

	if(state == CLOSED)
		connect();
}

void OverlayConnection::connect() {
	protocol::OverlayMessage message;
	message.mutable_header()->set_src_th(getMyTransportHash().toBinaryString());
	message.mutable_header()->set_dest_th(overlay_peer_ptr->getPeerTH().toBinaryString());
	message.mutable_header()->set_prio(message.header().RELIABLE);
	message.mutable_payload()->mutable_connection_part()->CopyFrom(generateConnectionPart(message.payload().connection_part().PUBKEY));
	sendMessage(message);
}

void OverlayConnection::disconnect() {
	state = CLOSED;
	OverlaySocket::getInstance()->m_connections.erase(overlay_peer_ptr->getPeerTH());
}

} /* namespace overlay */
} /* namespace p2pnet */
