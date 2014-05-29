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
#include "OverlaySocket.h"
#include "../transport/TransportSocket.h"
#include "../AsioIOService.h"
#include <algorithm>
#include <functional>
#include <boost/date_time.hpp>

namespace p2pnet {
namespace overlay {

OverlayConnection::OverlayConnection(const TH& th) :
		remote_key_lose_timer(AsioIOService::getIOService()),
		key_rotation_spam_timer(AsioIOService::getIOService()),
		key_rotation_spam_limit(getValue<long>("overlay.connection.key_rotation_spam_limit")) {
	remote_th = th;
	log() << "New Overlay Connection initiated with TH:" << remoteTH().toBase58() << std::endl;
}

OverlayConnection::~OverlayConnection() {
	disconnect();
}

OverlayKeyProvider* OverlayConnection::getKeyProvider() {
	return OverlaySocket::getInstance()->getKeyProvider();
}

TH OverlayConnection::localTH(){
	return getKeyProvider()->getTH();
}

const TH& OverlayConnection::remoteTH(){
	return remote_th;
}

crypto::PublicKeyDSA OverlayConnection::localPublicKey(){
	return getKeyProvider()->getPublicKey();
}

crypto::PrivateKeyDSA OverlayConnection::localPrivateKey(){
	return getKeyProvider()->getPrivateKey();
}

const crypto::PublicKeyDSA& OverlayConnection::remotePublicKey(){
	return remote_public_key;
}

void OverlayConnection::updateExpirationTime(boost::posix_time::ptime expiry_time) {
	if(expiry_time > remote_key_expiration_time)	// This is to prevent spoofing.
		remote_key_expiration_time = expiry_time;
}

void OverlayConnection::updateExpirationTime(const std::string& iso_time) {
	updateExpirationTime(boost::posix_time::from_iso_string(iso_time));
}

void OverlayConnection::updateLoseTime(boost::posix_time::ptime lost_time) {
	if(lost_time > remote_key_lose_time){
		remote_key_lose_time = lost_time;
		remote_key_lose_timer.expires_at(remote_key_lose_time);
		remote_key_lose_timer.async_wait([&](boost::system::error_code ec){if(!ec){disconnect();}});
	}
}

void OverlayConnection::updateLoseTime(const std::string& iso_time) {
	updateLoseTime(boost::posix_time::from_iso_string(iso_time));
}

void OverlayConnection::updateEndpoint(const transport::TransportSocketEndpoint& endpoint, bool verified){
	auto it = std::find(transport_endpoints.begin(), transport_endpoints.end(), endpoint);

	if(it != transport_endpoints.end())
		transport_endpoints.erase(it);

	if(verified)
		transport_endpoints.push_front(endpoint);
	else
		transport_endpoints.push_back(endpoint);
}

bool OverlayConnection::performLocalKeyRotation(const protocol::OverlayMessage& recv_message){
	auto& part = recv_message.payload().key_rotation_part();

	if(remotePublicKey().isPresent()){
		auto oldkey = crypto::PublicKeyDSA::fromBinaryString(part.signed_content().old_ecdsa_key());
		auto old_th = TH(oldkey);
		auto newkey = crypto::PublicKeyDSA::fromBinaryString(part.signed_content().new_ecdsa_key());
		auto new_th = TH(newkey);

		if(old_th == remoteTH()){	// Okay, this message is not alien. Checking.
			auto signed_content = part.signed_content().SerializeAsString();

			if(oldkey.verify(signed_content, part.old_signature()) && newkey.validate() && newkey.verify(signed_content, part.new_signature())){
				//This message is genuine. We are changing public key, connection in socket's map, position in K-buckets, expiration and lose timers.

				OverlaySocket::getInstance()->getDHT()->removeFromKBucket(this);
				OverlaySocket::getInstance()->movePeer(old_th, new_th);
				log() << "Changing " << old_th.toBase58() << " ~~> " << new_th.toBase58() << std::endl;
				remote_public_key = newkey;
				remote_th = new_th;
				OverlaySocket::getInstance()->getDHT()->registerInKBucket(this);

				return true;
			}
		}
	}//Else, we can't check these signatures, so we drop this message.
	return false;
}

void OverlayConnection::performRemoteKeyRotation(std::pair<crypto::PrivateKeyDSA, TH> previous_keys){
	if(state == State::ESTABLISHED){
		protocol::OverlayMessage message;
		message.mutable_header()->set_src_th(previous_keys.second.toBinaryString());
		message.mutable_header()->set_dest_th(remoteTH().toBinaryString());
		message.mutable_header()->set_prio(message.header().REALTIME);

		message.mutable_payload()->mutable_key_rotation_part()->CopyFrom(generateKeyRotationPart(previous_keys.first));
		sendRawMessage(message);
	}
}

void OverlayConnection::setState(const State& state_to_set){
	state = state_to_set;
	if(state_to_set == State::ESTABLISHED && !suspended_payloads.empty()){
		for(auto& payload : suspended_payloads){
			send(payload.first, payload.second);
		}
	}
}

void OverlayConnection::sendBinaryData(std::string data) {
	auto& transport_socket_connections = transport::TransportSocket::getInstance()->m_connections;

	std::shared_ptr<transport::TransportConnection> conn;

	// Searching for at least one "living" TransportConnection
	for(auto& conn_it : transport_endpoints){
		auto sock_it = transport_socket_connections.find(conn_it);
		if(sock_it->second->connected()){
			conn = sock_it->second;
			break;
		}
	}

	if(conn){
		conn->send(data);
		log() << "-> OverlayMessage: TH:" << remoteTH().toBase58() << std::endl;
	}else{
		// If there are no connections alive, we store messages to "suspended".
		// And, when they will be sent after some connections arrive.
		suspended_binary_data.push_front(data);
	}
}

void OverlayConnection::sendRawMessage(protocol::OverlayMessage send_message) {
	if(!(localTH().toBinaryString() == send_message.header().src_th())){
		auto our_historic_key = getKeyProvider()->getPrivateKey(send_message.header().src_th());
		if(our_historic_key != boost::none){
			if(send_message.header().prio() == send_message.header().RELIABLE){
				if(!send_message.payload().has_key_rotation_part()){
					send_message.mutable_payload()->mutable_key_rotation_part()->CopyFrom(generateKeyRotationPart(*our_historic_key));
				}
			}else if(!send_message.payload().has_key_rotation_part()){
				if(!key_rotation_spam_lock){
					key_rotation_spam_timer.expires_from_now(key_rotation_spam_limit);
					performRemoteKeyRotation(std::make_pair(*our_historic_key, send_message.header().src_th()));
					key_rotation_spam_lock = true;
					key_rotation_spam_timer.async_wait([&](const boost::system::error_code& ec) {if (!ec) key_rotation_spam_lock = false;});
				}
			}
		}
	}

	if(send_message.header().prio() == send_message.header().RELIABLE){
		send_message.mutable_header()->set_seq_num(seq_counter++);
		for(auto it = acked_messages.begin(); it != acked_messages.end(); it++){
			send_message.mutable_header()->mutable_ack_num()->Add(*it);
		}
		acked_messages.clear();	//TODO: dunno
	}

	sendBinaryData(send_message.SerializeAsString());
}

protocol::OverlayMessage OverlayConnection::generateReplySkel(const protocol::OverlayMessage& recv_message){
	protocol::OverlayMessage new_message;

	new_message.mutable_header()->set_src_th(recv_message.header().dest_th());
	new_message.mutable_header()->set_dest_th(recv_message.header().src_th());

	return new_message;
}

protocol::OverlayMessage_Payload_ConnectionPart OverlayConnection::generateConnectionPart(Stage for_stage){
	protocol::OverlayMessage_Payload_ConnectionPart conn_part;

	/* PUBKEY part */
	if(for_stage == Stage::PUBKEY || for_stage == Stage::PUBKEY_ACK){
		conn_part.set_stage( static_cast<protocol::OverlayMessage_Payload_ConnectionPart_ConnectionStage>(for_stage) );
		conn_part.set_src_ecdsa_pubkey(localPublicKey().toBinaryString());
		conn_part.set_expiration_time(getKeyProvider()->getExpirationISOTime());
		conn_part.set_lose_time(getKeyProvider()->getLoseISOTime());
	}

	/* ECDH part */
	else if(for_stage == Stage::ECDH || for_stage == Stage::ECDH_ACK){
		conn_part.set_stage( static_cast<protocol::OverlayMessage_Payload_ConnectionPart_ConnectionStage>(for_stage) );

		if(!session_ecdh_key.isPresent())
			session_ecdh_key = crypto::ECDH::generateNewKey();

		auto public_ecdh_component = session_ecdh_key.derivePublicKey();

		conn_part.set_src_ecdh_pubkey(public_ecdh_component);
		conn_part.set_signature(localPrivateKey().sign(public_ecdh_component));
	}

	/* AES part */
	else if(for_stage == Stage::AES){
		conn_part.set_stage( static_cast<protocol::OverlayMessage_Payload_ConnectionPart_ConnectionStage>(for_stage) );	// Consists of stage only
	}

	return conn_part;
}

protocol::OverlayMessage_Payload_KeyRotationPart OverlayConnection::generateKeyRotationPart(const crypto::PrivateKeyDSA& our_hist_key){
	protocol::OverlayMessage_Payload_KeyRotationPart part;

	std::string old_key_s = our_hist_key.derivePublicKey().toBinaryString();
	std::string new_key_s = localPublicKey().toBinaryString();

	part.mutable_signed_content()->set_old_ecdsa_key(old_key_s);
	part.mutable_signed_content()->set_new_ecdsa_key(new_key_s);

	part.mutable_signed_content()->set_expiration_time(getKeyProvider()->getExpirationISOTime());
	part.mutable_signed_content()->set_lose_time(getKeyProvider()->getLoseISOTime());

	auto signed_content = part.signed_content().SerializeAsString();

	part.set_old_signature(our_hist_key.sign(signed_content));
	part.set_new_signature(localPrivateKey().sign(signed_content));

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

	if((crypto::Hash(recv_dsa_pubkey) == remoteTH())	// So, we check if this message contains genuine ECDSA public key of connected TH.
			&& recv_dsa_pubkey.validate()){	// And then we validate this ECDSA public key using mathematical methods.
		log() << "Received ECDSA public key from: TH:" << remoteTH().toBase58() << std::endl;
		remote_public_key = recv_dsa_pubkey;

		updateExpirationTime(recv_message.payload().connection_part().expiration_time());
		updateLoseTime(recv_message.payload().connection_part().lose_time());
	}else{
		return;	// Drop. TODO MessageReject.
	}

	auto reply = generateReplySkel(recv_message);

	if(recv_message.payload().connection_part().stage() == recv_message.payload().connection_part().PUBKEY){
		/* We need to send back PUBKEY_ACK */
		reply.mutable_header()->set_prio(reply.header().RELIABLE);
		reply.mutable_payload()->mutable_connection_part()->CopyFrom(generateConnectionPart(Stage::PUBKEY_ACK));

		setState(State::PUBKEY_RECEIVED);

		sendRawMessage(reply);
	}else if(static_cast<Stage>(recv_message.payload().connection_part().stage()) == Stage::PUBKEY_ACK){
		/* We need to send back ECDH */
		reply.mutable_header()->set_prio(reply.header().RELIABLE);
		reply.mutable_payload()->mutable_connection_part()->CopyFrom(generateConnectionPart(Stage::ECDH));

		setState(State::ECDH_SENT);

		sendRawMessage(reply);
	}
}

void OverlayConnection::processConnectionPartECDH(const protocol::OverlayMessage& recv_message){
	if(!session_ecdh_key.isPresent())
		session_ecdh_key = crypto::ECDH::generateNewKey();
	auto salt_v = remoteTH() ^ localTH();
	auto derived_aes_string = session_ecdh_key.deriveSymmetricKey(crypto::AES::vectorSize(),
			recv_message.payload().connection_part().src_ecdh_pubkey(), std::string(salt_v.begin(), salt_v.end())
			);
	session_aes_key = crypto::AES::fromBinaryString(derived_aes_string);

	log() << "Received ECDH public key from: TH:" << remoteTH().toBase58() << std::endl;

	auto reply = generateReplySkel(recv_message);

	if(recv_message.payload().connection_part().stage() == recv_message.payload().connection_part().ECDH && state == State::PUBKEY_RECEIVED){
		/* We need to send back ECDH_ACK */
		reply.mutable_header()->set_prio(reply.header().RELIABLE);
		reply.mutable_payload()->mutable_connection_part()->CopyFrom(generateConnectionPart(Stage::ECDH_ACK));

		setState(State::ECDH_RECEIVED);

		sendRawMessage(reply);
	}else if(recv_message.payload().connection_part().stage() == recv_message.payload().connection_part().ECDH_ACK && state == State::ECDH_SENT){
		/* We need to send back AES */
		reply.mutable_header()->set_prio(reply.header().RELIABLE);
		protocol::OverlayMessage_Payload not_encrypted_payload;
		not_encrypted_payload.mutable_connection_part()->CopyFrom(generateConnectionPart(Stage::AES));
		reply.set_encrypted_payload(encryptPayload(not_encrypted_payload));

		sendRawMessage(reply);

		setState(State::ESTABLISHED);
		log() << "AES encrypted connection with TH:" << remoteTH().toBase58() << " established" << std::endl;
	}
}

void OverlayConnection::processConnectionPartAES(const protocol::OverlayMessage& recv_message,
		const protocol::OverlayMessage_Payload& decrypted_payload){
	setState(State::ESTABLISHED);
	log() << "AES encrypted connection with TH:" << remoteTH().toBase58() << " established" << std::endl;
}

std::string OverlayConnection::encryptPayload(const protocol::OverlayMessage_Payload& payload){
	return session_aes_key.encrypt(payload.SerializeAsString());
}

bool OverlayConnection::connected() const {
	return state == State::ESTABLISHED;
}

void OverlayConnection::send(const protocol::OverlayMessage_Payload& send_payload, Priority prio) {
	if(state == State::ESTABLISHED){
		protocol::OverlayMessage message;
		message.mutable_header()->set_src_th(localTH().toBinaryString());
		message.mutable_header()->set_dest_th(remoteTH().toBinaryString());
		message.mutable_header()->set_prio((protocol::OverlayMessage_Header_MessagePriority)prio);
		message.set_encrypted_payload(encryptPayload(send_payload));
		sendRawMessage(message);
	}else{
		if(prio == Priority::RELIABLE){
			suspended_payloads.push_back(std::make_pair(send_payload, prio));
		}else if(prio == Priority::REALTIME){
			// TODO: We need a new queue for use with realtime messages.
		}
	}
}

void OverlayConnection::process(const protocol::OverlayMessage& recv_message, const transport::TransportSocketEndpoint& from) {
	updateEndpoint(from);

	std::shared_ptr<crypto::PrivateKeyDSA> our_historic_ecdsa_privkey;

	TH src_th = TH::fromBinaryString(recv_message.header().src_th());
	TH dest_th = TH::fromBinaryString(recv_message.header().dest_th());

	log() << "<- OverlayMessage: TH:" << src_th.toBase58() << std::endl;

	if(getKeyProvider()->getPrivateKey(dest_th) != boost::none){
		// So, this message is for us.
		/* Key Rotation stage */
		if(recv_message.payload().has_key_rotation_part())
			if(performLocalKeyRotation(recv_message))
				updateEndpoint(from, true);
		/* Encryption stage */
		if(recv_message.has_encrypted_payload() && session_aes_key.isPresent()){
			protocol::OverlayMessage_Payload decrypted_payload;
			auto decrypted_payload_s = session_aes_key.decrypt(recv_message.encrypted_payload());
			if(decrypted_payload.ParseFromString(decrypted_payload_s)){
				/* Encrypted processing */
				updateEndpoint(from, true);
				processConnectionPart(recv_message, decrypted_payload);
				// DHT
				//OverlaySocket::getInstance()->dht_service.process(src_th, recv_message.payload().GetExtension(protocol::dht_part));
			}else{
				// TODO: Reconnect
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
	updateEndpoint(from);

	if(state == State::CLOSED)	// TODO: Here go "privacy settings"
		connect();
}

void OverlayConnection::connect() {
	protocol::OverlayMessage message;
	message.mutable_header()->set_src_th(localTH().toBinaryString());
	message.mutable_header()->set_dest_th(remoteTH().toBinaryString());
	message.mutable_header()->set_prio(message.header().RELIABLE);
	message.mutable_payload()->mutable_connection_part()->CopyFrom(generateConnectionPart(Stage::PUBKEY));
	sendRawMessage(message);
}

void OverlayConnection::disconnect() {
	state = State::CLOSED;
}

} /* namespace overlay */
} /* namespace p2pnet */
