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
			auto sock_id = registerNewSocket();

			APIMessage message_reply;
			message_reply.set_type(APIMessage::REGISTER_SOCKET_CALLBACK);
			message_reply.set_socket_id(sock_id);
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
	}
}

uint32_t APISession::registerNewSocket() {
	if(next_id == 0)
		return 0;
	uint32_t socket_id = next_id;
	auto it_pair = endpoints.insert(std::make_pair(socket_id, std::shared_ptr<endpoint::LocalEndpoint>()));

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

	return socket_id;
}

void APISession::unregisterSocket(uint32_t sock_id) {
	auto it = endpoints.find(sock_id);
	endpoints.erase(it);
	log() << "Unegistered socket #" << sock_id << std::endl;
	if(next_id > sock_id || next_id == 0){
		next_id = sock_id;
	}
}

void APISession::dropSession() {
	parent_api_server->dropSession(this);
}

} /* namespace api */
} /* namespace p2pnet */
