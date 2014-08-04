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

OverlayConnection::OverlayConnection(OverlayNode* node) : key_rotation_spam_timer(AsioIOService::getIOService()),
		key_rotation_spam_limit(getValue<long>("overlay.connection.key_rotation_spam_limit")) {
	node_ptr = node;
	log() << "New Overlay Connection initiated with TH:" << remoteTH().toBase58() << std::endl;
}

OverlayConnection::~OverlayConnection() {
	disconnect();
}

void OverlayConnection::performRemoteKeyRotation(std::pair<crypto::PrivateKeyDSA, TH> previous_keys){
	if(state == State::ESTABLISHED){
		OverlayMessage message = genMessageSkel(previous_keys.second, remoteTH(), Priority::RELIABLE);
		Handshake handshake;

		handshake.set_stage(handshake.PUBKEY_ROTATION);
		handshake.mutable_pubkey()->CopyFrom(genPubkeyStage(handshake.PUBKEY_ROTATION, previous_keys.first));

		sendMessage(message);
	}
}

void OverlayConnection::setState(const State& state_to_set){
	state = state_to_set;

	if(state_to_set == State::ESTABLISHED){
		node_ptr->registerDHT();
	}else if(state_to_set == State::CLOSED){
		node_ptr->unregisterDHT();
	}

	if(state_to_set == State::ESTABLISHED && !suspended_payloads.empty()){
		for(auto& payload : suspended_payloads){
			send(payload.first, payload.second);
		}
	}
}

void OverlayConnection::sendMessage(OverlayMessage send_message) {

	/*
	if(localTH().toBinaryString() != send_message.header().src_th()){
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

	//

	auto& transport_socket_connections = transport::TransportSocket::getInstance()->m_connections;

	std::shared_ptr<transport::TransportConnection> conn;

	// Searching for at least one "living" TransportConnection
	for(auto& conn_it : node_ptr->getTransportEndpoints()){
		auto sock_it = transport_socket_connections.find(conn_it);
		if(sock_it->second->connected()){
			conn = sock_it->second;
			break;
		}
	}

	if(conn){
		conn->send(send_message.SerializeAsString());
		log() << "-> OverlayMessage: TH:" << remoteTH().toBase58() << std::endl;
	}else{
		// If there are no connections alive, we store messages to "suspended".
		// And, when they will be sent after some connections arrive.
		suspended_messages.push_front(send_message);
		// Then we will try to find additional nodes
		OverlaySocket::getInstance()->getDHT()->findNodeIterative(remoteTH());
	}*/
}

OverlayMessage OverlayConnection::genMessageSkel(boost::optional<TH> src,
		boost::optional<TH> dest,
		boost::optional<Priority> prio){
	OverlayMessage new_message;

	new_message.mutable_header()->set_src_th(src->toBinaryString());
	new_message.mutable_header()->set_dest_th(dest->toBinaryString());
	new_message.mutable_header()->set_prio((OverlayMessage::Header::MessagePriority)*prio);

	return new_message;
}

OverlayMessage OverlayConnection::genMessageSkel(const OverlayMessage_Header& reply_to){
	boost::optional<TH> src, dest;
	boost::optional<Priority> prio;

	if(reply_to.has_dest_th()){
		src = TH::fromBinaryString(reply_to.dest_th());
	}else{src = boost::none;}

	if(reply_to.has_src_th()){
		dest = TH::fromBinaryString(reply_to.src_th());
	}else{dest = boost::none;}

	if(reply_to.has_prio()){
		prio = (Priority)reply_to.prio();
	}else{prio = boost::none;}

	return genMessageSkel(src, dest, prio);
}



Handshake_PubkeyStage OverlayConnection::genPubkeyStage(Handshake::Stage for_stage, boost::optional<const crypto::PrivateKeyDSA&> our_hist_key){
	Handshake_PubkeyStage stage;

	std::string new_key_s = localPublicKey().toBinaryString();
	stage.mutable_signed_content()->set_new_ecdsa_key(new_key_s);

	stage.mutable_signed_content()->set_expiration_time(system_clock::to_time_t(getKeyProvider()->getExpirationTime()));
	stage.mutable_signed_content()->set_lose_time(system_clock::to_time_t(getKeyProvider()->getLoseTime()));

	if(for_stage == Handshake::PUBKEY_ROTATION){
		std::string old_key_s = our_hist_key->derivePublicKey().toBinaryString();
		stage.mutable_signed_content()->set_old_ecdsa_key(old_key_s);

		auto signed_content = stage.signed_content().SerializeAsString();

		stage.set_old_signature(our_hist_key->sign(signed_content));
		stage.set_new_signature(localPrivateKey().sign(signed_content));
	}

	return stage;
}

Handshake_ECDHStage OverlayConnection::genECDHStage(){
	Handshake_ECDHStage stage;

	if(!session_ecdh_key.isPresent())
		session_ecdh_key = crypto::ECDH::generateNewKey();

	auto public_ecdh_component = session_ecdh_key.derivePublicKey();

	stage.set_src_ecdh_pubkey(public_ecdh_component);
	stage.set_signature(localPrivateKey().sign(public_ecdh_component));

	return stage;
}

void OverlayConnection::processHandshake(const OverlayMessage_Header& header, std::string serialized_payload) {
	Handshake handshake;
	handshake.ParseFromString(serialized_payload);

	switch(handshake.stage()){
		case Handshake_Stage_PUBKEY:
		case Handshake_Stage_PUBKEY_ACK:
		case Handshake_Stage_PUBKEY_ROTATION:
			processPubkeyStage(header, handshake);
			break;

		case Handshake_Stage_ECDH:
		case Handshake_Stage_ECDH_ACK:
			processECDHStage(header, handshake);
			break;
	}
}

void OverlayConnection::processPubkeyStage(const OverlayMessage_Header& header, Handshake handshake_payload){
	auto new_ecdsa_key = crypto::PublicKeyDSA::fromBinaryString(handshake_payload.pubkey().signed_content().new_ecdsa_key());

	if(crypto::Hash(new_ecdsa_key) == remoteTH()){
		node_ptr->setPublicKey(new_ecdsa_key);
	}else if(handshake_payload.stage() == handshake_payload.PUBKEY_ROTATION &&
			handshake_payload.pubkey().signed_content().has_old_ecdsa_key()){
		auto signed_content = handshake_payload.pubkey().signed_content().SerializeAsString();
		auto old_ecdsa_key = crypto::PublicKeyDSA::fromBinaryString(handshake_payload.pubkey().signed_content().old_ecdsa_key());

		if(old_ecdsa_key.verify(signed_content, handshake_payload.pubkey().old_signature()) &&
				new_ecdsa_key.validate() &&
				new_ecdsa_key.verify(signed_content, handshake_payload.pubkey().new_signature())){
			// So, the new key is good
			node_ptr->setPublicKey(new_ecdsa_key);
		}
	}else{
		return;
	}

	if(handshake_payload.stage() != handshake_payload.PUBKEY_ROTATION){
		auto reply = genMessageSkel(header);
		Handshake new_handshake_payload;

		switch(handshake_payload.stage()){
			case Handshake_Stage_PUBKEY:
				new_handshake_payload.mutable_pubkey()->CopyFrom(genPubkeyStage(Handshake_Stage_PUBKEY_ACK));
				break;
			case Handshake_Stage_PUBKEY_ACK:
				new_handshake_payload.mutable_pubkey()->CopyFrom(genPubkeyStage(Handshake_Stage_PUBKEY_ACK));
				break;
			default:
		}
		addPayload(new_handshake_payload.SerializeAsString(), PayloadType::HANDSHAKE, reply);
	}
}

void OverlayConnection::processECDHStage(const OverlayMessage_Header& header, Handshake handshake_payload){
	if(!session_ecdh_key.isPresent())
		session_ecdh_key = crypto::ECDH::generateNewKey();

	auto salt_v = (remoteTH() ^ localTH()).toBinaryString();
	auto derived_aes_string = session_ecdh_key.deriveSymmetricKey(crypto::AES::vectorSize(),
			handshake_payload.ecdh().src_ecdh_pubkey(), salt_v);
	session_aes_key = crypto::AES::fromBinaryString(derived_aes_string);

	log() << "Received ECDH public key from: TH:" << remoteTH().toBase58() << std::endl;

	auto reply = genMessageSkel(header);
	Handshake new_handshake_payload;

	if(handshake_payload.stage() == handshake_payload.ECDH && state == State::PUBKEY_RECEIVED){
		/* We need to send back ECDH_ACK */
		new_handshake_payload.mutable_ecdh()->CopyFrom(genECDHStage());

		addPayload(new_handshake_payload.SerializeAsString(), PayloadType::HANDSHAKE, reply);
		sendMessage(reply);
	}

	setState(State::ESTABLISHED);
	log() << "AES encrypted connection with TH:" << remoteTH().toBase58() << " established" << std::endl;
}

bool OverlayConnection::connected() const {
	return state == State::ESTABLISHED;
}

void OverlayConnection::send(const OverlayMessage_Payload& send_payload, Priority prio) {
	if(state == State::ESTABLISHED){
		OverlayMessage message;
		message.mutable_header()->set_src_th(localTH().toBinaryString());
		message.mutable_header()->set_dest_th(remoteTH().toBinaryString());
		message.mutable_header()->set_prio((OverlayMessage_Header_MessagePriority)prio);
		message.set_encrypted_payload(encryptPayload(send_payload));
		sendMessage(message);
	}else{
		if(prio == Priority::RELIABLE){
			suspended_payloads.push_back(std::make_pair(send_payload, prio));
		}else if(prio == Priority::REALTIME){
			// TODO: We need a new queue for use with realtime messages.
		}
		if(state == State::CLOSED){
			connect();
		}
	}
}

void OverlayConnection::process(const OverlayMessage& recv_message, const transport::TransportSocketEndpoint& from) {
	node_ptr->updateEndpoint(from);

	std::shared_ptr<crypto::PrivateKeyDSA> our_historic_ecdsa_privkey;

	TH src_th = TH::fromBinaryString(recv_message.header().src_th());
	TH dest_th = TH::fromBinaryString(recv_message.header().dest_th());

	log() << "<- OverlayMessage: TH:" << src_th.toBase58() << std::endl;

	if(getKeyProvider()->getPrivateKey(dest_th) != boost::none){
		// So, this message is for us.
		/* Key Rotation stage */
		if(recv_message.payload().has_key_rotation_part())
			if(performLocalKeyRotation(recv_message))
				node_ptr->updateEndpoint(from, true);
		/* Encryption stage */
		if(recv_message.has_encrypted_payload() && session_aes_key.isPresent()){
			OverlayMessage_Payload decrypted_payload;
			auto decrypted_payload_s = session_aes_key.decrypt(recv_message.encrypted_payload());
			if(decrypted_payload.ParseFromString(decrypted_payload_s)){
				/* Encrypted processing */
				node_ptr->updateEndpoint(from, true);
				processHandshake(recv_message, decrypted_payload);
				// DHT
				OverlaySocket::getInstance()->getDHT()->process(src_th, recv_message.payload().GetExtension(dht_part));
			}else{
				// TODO: Reconnect
			}
		}else{
			/* Open (unencrypted) processing */
			processHandshake(recv_message);
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

void OverlayConnection::connect() {
	OverlayMessage message;
	message.mutable_header()->set_src_th(localTH().toBinaryString());
	message.mutable_header()->set_dest_th(remoteTH().toBinaryString());
	message.mutable_header()->set_prio(message.header().RELIABLE);
	message.mutable_payload()->mutable_connection_part()->CopyFrom(generateConnectionPart(Stage::PUBKEY));
	sendMessage(message);
}

void OverlayConnection::disconnect() {
	state = State::CLOSED;
}

} /* namespace overlay */
} /* namespace p2pnet */
