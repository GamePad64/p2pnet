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

#include "../p2p/NodeManager.h"
#include "../p2p/Node.h"
#include "../p2p/Socket.h"

#include "../../common/crypto/PrivateKeyDSA.h"
#include "../../common/api/APIMessage.pb.h"

namespace p2pnet {
namespace api {

APISession::APISession(APIServer* parent) {
	parent_api_server = parent;
	next_node_id = 1;
	log() << "New API session started" << std::endl;
}
APISession::~APISession() {
	log() << "API session shut down" << std::endl;
}

void APISession::process(APIMessage message) {
	switch(message.type()){
	case APIMessage::NODE_REGISTER: NodeRegister(message); break;
	case APIMessage::NODE_UNREGISTER: NodeUnRegister(message); break;
	case APIMessage::NODE_CONNECT: NodeConnect(message); break;
	case APIMessage::NODE_ACCEPT: NodeAccept(message); break;
	case APIMessage::NODE_LISTEN: NodeListen(message); break;
	case APIMessage::NODE_BIND: NodeBind(message); break;
	case APIMessage::SOCKET_UNREGISTER: SocketUnRegister(message); break;
	default: return;
	}
}

void APISession::NodeRegister(APIMessage message) {
	APIMessage reply;
	reply.set_type(reply.NODE_REGISTER_CALLBACK);

	if(next_node_id == 0){
		// Problem. We reached the limit of nodes.
		// TODO: error handling.
		reply.mutable_error()->set_error_code(1);
		reply.mutable_error()->set_error_what("Node limit exceeded.");
		send(reply);
		return;
	}
	uint32_t node_id = next_node_id;

	// Registration itself.
	auto new_node = p2p::NodeManager::getInstance()->createNode(this);

	node_t new_node_t;
	new_node_t.node = new_node;
	new_node_t.next_socket_id = 1;

	auto nodes_inserted_it = nodes.insert(std::make_pair(node_id, new_node_t));

	// Sending callback
	APIMessage_NodeRegisterCallback callback;
	callback.set_node_id(node_id);
	reply.set_api_message(callback.SerializeAsString());
	send(reply);

	log() << "Registered socket #" << node_id << std::endl;

	// Computing next node_id
	next_node_id++;	// If it turns into zero, than this was the last node we can create on this APISession;

	auto new_iterator = nodes_inserted_it.first;
	new_iterator++;

	while(next_node_id != 0 && new_iterator != nodes.end()){
		if(new_iterator->first <= next_node_id){
			next_node_id++;
			new_iterator++;
		}else{
			break;
		}
	}
}

void APISession::NodeUnRegister(APIMessage message) {
	APIMessage::NodeUnRegister api_message;
	api_message.ParseFromString(message.api_message());

	auto it = nodes.find(api_message.node_id());
	p2p::NodeManager::getInstance()->destroyNode(it->second.node);
	nodes.erase(it);

	log() << "Unegistered node #" << api_message.node_id() << std::endl;
	if(next_node_id > api_message.node_id() || next_node_id == 0){
		next_node_id = api_message.node_id();
	}//TODO: Callback
}

void APISession::NodeConnect(APIMessage message) {
	throw;
}

void APISession::NodeAccept(APIMessage message) {
	throw;
}

void APISession::NodeListen(APIMessage message) {
	APIMessage::NodeListen api_message;
	api_message.ParseFromString(message.api_message());

	auto node_it = nodes.find(api_message.node_id());
	if(node_it != nodes.end()){
		node_it->second.node->listen(api_message.max_connections());
	}else{
		// TODO: send error to client. Unknown socket.
	}
}

void APISession::NodeBind(APIMessage message) {
}

void APISession::SocketUnRegister(APIMessage message) {
}

void APISession::dropSession() {
	parent_api_server->dropSession(this);
}

} /* namespace api */
} /* namespace p2pnet */
