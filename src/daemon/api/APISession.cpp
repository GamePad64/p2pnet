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
#include "APISession.h"
#include "APIServer.h"

#include "../endpoint/EndpointManager.h"

#include "../../common/crypto/PrivateKeyDSA.h"
#include "../../common/api/APIMessage.pb.h"

namespace p2pnet {
namespace api {

APISession::APISession(APIServer* parent) {
	parent_api_server = parent;
	socket_count = 0;
	next_id = 1;
	log() << "New API session started" << std::endl;
}
APISession::~APISession() {
	log() << "API session shut down" << std::endl;
}

void APISession::process(APIMessage message) {
	switch(message.type()){
	case APIMessage::REGISTER_SOCKET:	{
			auto socket = registerNewSocket();

			APIMessage message_reply;
			message_reply.set_type(APIMessage::REGISTER_SOCKET_CALLBACK);
			message_reply.set_socket_id(socket.first);
			send(message_reply);
		}
		break;
	case APIMessage::UNREGISTER_SOCKET:	{
			unregisterSocket(message.socket_id());

			APIMessage message_reply;
			message_reply.set_type(APIMessage::UNREGISTER_SOCKET_CALLBACK);
			message_reply.set_socket_id(message.socket_id());
			send(message_reply);
		}
		break;
	case APIMessage::BIND_SOCKET:	{
			auto private_key = crypto::PrivateKeyDSA::fromBase58(message.privkey_b58());
			bind(message.socket_id(), private_key);

			APIMessage message_reply;
			message_reply.set_type(APIMessage::BIND_SOCKET_CALLBACK);
			message_reply.set_socket_id(message.socket_id());
			send(message_reply);
		}
		break;
	case APIMessage::LISTEN:	{
			auto max_connections = message.max_connections();
			listen(message.socket_id(), max_connections);

			APIMessage message_reply;
			message_reply.set_type(APIMessage::BIND_SOCKET_CALLBACK);
			message_reply.set_socket_id(message.socket_id());
			send(message_reply);
		}
		break;
	case APIMessage::REGISTER_SOCKET_CALLBACK:
	case APIMessage::UNREGISTER_SOCKET_CALLBACK:
	case APIMessage::BIND_SOCKET_CALLBACK:
	case APIMessage::LISTEN_CALLBACK:
		break;
	}
}

std::pair<uint32_t, std::shared_ptr<endpoint::EndpointSocket>> APISession::registerNewSocket() {
	if(next_id == 0)
		return std::make_pair(0, nullptr);
	uint32_t socket_id = next_id;

	// Registration itself.
	auto new_socket = std::make_shared<endpoint::EndpointSocket>(this);
	auto it_pair = endpoints.insert(std::make_pair(socket_id, new_socket));
	endpoint::EndpointManager::getInstance()->registerSocket(new_socket);

	log() << "Registered socket #" << socket_id << std::endl;

	socket_count++;

	next_id++;	// If it turns into zero, than this was the last socket we can create;

	auto new_iterator = it_pair.first;
	new_iterator++;

	while(next_id != 0 && new_iterator != endpoints.end()){
		if(new_iterator->first <= next_id){
			next_id++;
			new_iterator++;
		}else{
			break;
		}
	}

	return std::make_pair(socket_id, new_socket);
}

void APISession::unregisterSocket(uint32_t sock_id) {
	auto it = endpoints.find(sock_id);
	endpoint::EndpointManager::getInstance()->unregisterSocket(it->second);
	endpoints.erase(it);

	log() << "Unegistered socket #" << sock_id << std::endl;
	if(next_id > sock_id || next_id == 0){
		next_id = sock_id;
	}
}

void APISession::bind(uint32_t sock_id, crypto::PrivateKeyDSA private_key) {
	auto it = endpoints.find(sock_id);
	if(it != endpoints.end()){
		it->second->bind(private_key);
	}//TODO: else error.
}

void APISession::listen(uint32_t sock_id, uint32_t max_connections) {
	auto it = endpoints.find(sock_id);
	if(it != endpoints.end()){
		it->second->listen(max_connections);
	}//TODO: else error.
}

void APISession::dropSession() {
	parent_api_server->dropSession(this);
}

} /* namespace api */
} /* namespace p2pnet */
