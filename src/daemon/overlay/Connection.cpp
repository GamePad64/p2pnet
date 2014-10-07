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
#include "Connection.h"
#include "Socket.h"
#include "../errors/Errors.h"
#include "../transport/Socket.h"
#include "../AsioIOService.h"

#include <boost/date_time.hpp>
#include <boost/optional.hpp>

#include <algorithm>
#include <functional>

namespace p2pnet {
namespace overlay {

Connection::Connection(std::weak_ptr<Socket> parent_socket, TH th) : parent_socket(parent_socket) {
	node_info.th = th;
	log() << "New overlay::Connection initiated with TH:" << th.toBase58() << std::endl;
}

Connection::~Connection() {
	disconnect();
}

void Connection::setState(const State& state_to_set){
	state = state_to_set;

	if(state_to_set == State::ESTABLISHED){
		onConnect();
	}else if(state_to_set == State::CLOSED){
		onDisconnect();
	}
}

//void Connection::sendMessage(OverlayMessage send_message) {


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
//}

OverlayMessage Connection::genMessageSkel(const TH& from, const TH& to){
	OverlayMessage new_message;

	new_message.mutable_header()->set_src_th(from.toBinaryString());
	new_message.mutable_header()->set_dest_th(to.toBinaryString());

	return new_message;
}

OverlayMessage Connection::genMessageSkel(){
	return genMessageSkel(localKeyInfo().th, remoteKeyInfo().th);
}

void Connection::send(const OverlayMessage& message, Socket::SendCallback callback) {
	parent_socket.lock()->send(message, callback, shared_from_this());
}

void Connection::send(const OverlayMessage_Payload& payload, Socket::SendCallback callback, bool encrypted, bool force_non_connected) {
	OverlayMessage message = genMessageSkel();	// Construct message
	OverlayMessage_MultiPayload multipayload;
	multipayload.add_payload()->CopyFrom(payload);

	if(encrypted && connected()){
		message.mutable_body()->set_encrypted_multipayload(session_aes_key.encrypt(multipayload.SerializeAsString()));
		send(message, callback);
	}else if(!encrypted && (connected() || force_non_connected)){
		message.mutable_body()->mutable_open_multipayload()->CopyFrom(multipayload);
		send(message, callback);
	}else{
		AsioIOService::getIOService().post(std::bind(callback, not_connected, shared_from_this(), "", 0));
	}
}

void Connection::process(std::shared_ptr<transport::Connection> origin, const OverlayMessage_Header& header, const OverlayMessage_Body& body) {
	updateTransport(origin);

	TH src_th = TH::fromBinaryString(header.src_th());
	TH dest_th = header.has_dest_th() ? TH::fromBinaryString(header.dest_th()) : TH();

	log() << "<- OverlayMessage: TH:" << src_th.toBase58() << std::endl;

	if(dest_th){
		std::list<std::pair<OverlayMessage_Payload, bool>> payloads;

		for(auto payload : body.open_multipayload().payload()){
			payloads.push_back(std::make_pair(payload, false));
		}

		if(connected()){
			OverlayMessage_MultiPayload encrypted_multipayload;
			encrypted_multipayload.ParseFromString(session_aes_key.decrypt(body.encrypted_multipayload()));

			for(auto payload : encrypted_multipayload){
				payloads.push_back(std::make_pair(payload, true));
			}
		}

		// Firing processors
		for(auto payload_pair : payloads){
			for(auto processor : parent_socket.lock()->getProcessors((PayloadType)payload_pair.first.type())){
				if(!(processor->isEncryptionMandatory() && !payload_pair.second)){
					processor->process(shared_from_this(), header, payload_pair.first);
				}
			}
		}
	}
}

/*void Connection::process(std::shared_ptr<transport::Connection> origin, const OverlayMessage_Header& header, const OverlayMessage_Body& body) {

	node_ptr->updateEndpoint(from);

	std::shared_ptr<crypto::PrivateKeyDSA> our_historic_ecdsa_privkey;

	TH src_th = TH::fromBinaryString(recv_message.header().src_th());
	TH dest_th = TH::fromBinaryString(recv_message.header().dest_th());

	log() << "<- OverlayMessage: TH:" << src_th.toBase58() << std::endl;

	if(getKeyProvider()->getPrivateKey(dest_th) != boost::none){
		// So, this message is for us.
		// Key Rotation stage
		if(recv_message.payload().has_key_rotation_part())
			if(performLocalKeyRotation(recv_message))
				node_ptr->updateEndpoint(from, true);
		// Encryption stage
		if(recv_message.has_encrypted_payload() && session_aes_key.isPresent()){
			OverlayMessage_Payload decrypted_payload;
			auto decrypted_payload_s = session_aes_key.decrypt(recv_message.encrypted_payload());
			if(decrypted_payload.ParseFromString(decrypted_payload_s)){
				// Encrypted processing
				node_ptr->updateEndpoint(from, true);
				processHandshake(recv_message, decrypted_payload);
				// DHT
				Socket::getInstance()->getDHT()->process(src_th, recv_message.payload().GetExtension(dht_part));
			}else{
				// TODO: Reconnect
			}
		}else{
			// Open (unencrypted) processing
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
}*/

void Connection::connect(Socket::ConnectCallback callback) {
	OverlayMessage message;
	message.mutable_header()->set_src_th(localTH().toBinaryString());
	message.mutable_header()->set_dest_th(remoteTH().toBinaryString());
	message.mutable_header()->set_prio(message.header().RELIABLE);
	message.mutable_payload()->mutable_connection_part()->CopyFrom(generateConnectionPart(Stage::PUBKEY));
	sendMessage(message);
}

void Connection::disconnect() {
	setState(State::CLOSED);
}

void Connection::updateTransport(std::shared_ptr<transport::Connection> tconn, bool verified){
	auto it = std::find(transport_connections.begin(), transport_connections.end(), tconn);

	if(it != transport_connections.end())
		transport_connections.erase(it);

	if(verified)
		transport_connections.push_front(tconn);
	else
		transport_connections.push_back(tconn);
}

void Connection::updateExpirationTime(std::chrono::system_clock::time_point expiry_time) {
	if(expiry_time > node_info.expiration_time)	// This is to prevent spoofing.
		node_info.expiration_time = expiry_time;
}

void Connection::updateLoseTime(std::chrono::system_clock::time_point lost_time) {
	node_info.lose_time = lost_time;
}

} /* namespace overlay */
} /* namespace p2pnet */
