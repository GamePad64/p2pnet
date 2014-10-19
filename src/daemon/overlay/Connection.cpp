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
#include "../../p2pnet.h"
#include "../transport/Socket.h"
#include "../AsioIOService.h"

#include <boost/date_time.hpp>
#include <boost/optional.hpp>

#include <algorithm>
#include <functional>

namespace p2pnet {
namespace overlay {

Connection::Connection(std::weak_ptr<Socket> parent_socket, TH th) : parent_socket(parent_socket), callback_timer(AsioIOService::getIOService()) {
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

void Connection::onConnect(){

}

void Connection::onDisconnect(){

}

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

void Connection::send(const OverlayMessage_Payload& payload, Socket::SendCallback callback, PayloadPriority prio, bool encrypted, bool force_non_connected) {
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
		AsioIOService::getIOService().post(std::bind(callback, (int)P2PError::not_connected, shared_from_this(), "", 0));
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

			for(auto payload : encrypted_multipayload.payload()){
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

			if((PayloadType)payload_pair.first.type() == PayloadType::HANDSHAKE){
				Handshake handshake_content;
				handshake_content.ParseFromString(payload_pair.first.content());
				processHandshake(header, handshake_content);
			}
		}
	}
}

void Connection::processHandshake(const OverlayMessage_Header& header, const Handshake& handshake){

}

void Connection::connectTimeout(std::shared_ptr<Connection> connection, Socket::ConnectCallback callback, const boost::system::error_code& ec){
	if(!ec){
		callback((int)P2PError::timed_out, connection);
	}
}

void Connection::connect(Socket::ConnectCallback callback) {
	if(state == State::ESTABLISHED){
		callback(0, shared_from_this());
	}else{
		auto timeout_time = std::chrono::system_clock::now() + std::chrono::seconds(getValue<unsigned int>("overlay.connection.timeout"));
		connect_callbacks.push_back(std::make_pair(callback, timeout_time));
		callback_timer.expires_at(timeout_time);
		callback_timer.async_wait(std::bind(&Connection::connectTimeout, this, shared_from_this(), callback, std::placeholders::_1));

		if(state == State::FOUND_TRANSPORT){
			OverlayMessage_Payload handshake_payload;
			Handshake handshake_content;
			handshake_content.set_stage(handshake_content.INIT);
			handshake_content.mutable_signed_handshake()->mutable_key_info()->CopyFrom(localKeyInfo().toProtobuf());

			session_ecdh_key.generateKey();
			handshake_content.mutable_signed_handshake()->set_ecdh_key(session_ecdh_key.derivePublicKey());
			handshake_content.set_new_key_signature(localKeyInfo().private_key.sign(handshake_content.signed_handshake().SerializeAsString()));

			handshake_payload.set_type((uint32_t)PayloadType::HANDSHAKE);
			handshake_payload.set_content(handshake_content.SerializeAsString());

			send(handshake_payload, [](int, std::shared_ptr<Connection>, std::string, int){}, PayloadPriority::RELIABLE, false, true);// check state before. Do not spam. And use dht if no transport
			setState(State::HANDSHAKE_INIT_SENT);
		}
	}
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

	if(state == State::SEARCHING_TRANSPORT){
		setState(State::FOUND_TRANSPORT);
	}
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
