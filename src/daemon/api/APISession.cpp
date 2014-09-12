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
#include "APIMessage.pb.h"

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

void APISession::process(APIMessage request_message) {
	switch(request_message.type()){
	case APIMessage::NODE_REGISTER: NodeRegister(request_message); break;
	case APIMessage::NODE_UNREGISTER: NodeUnRegister(request_message); break;
	case APIMessage::NODE_CONNECT: NodeConnect(request_message); break;
	case APIMessage::NODE_ACCEPT: NodeAccept(request_message); break;
	case APIMessage::NODE_LISTEN: NodeListen(request_message); break;
	case APIMessage::NODE_BIND: NodeBind(request_message); break;
	case APIMessage::SOCKET_UNREGISTER: SocketUnRegister(request_message); break;
	default: return;
	}
}

void APISession::NodeRegister(APIMessage request_message) {
	if(next_node_id == 0){
		// Problem. We reached the limit of nodes.
		remoteThrow(request_message, std::error_condition((int)P2PError::too_many_nodes, P2PErrorCategory));
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
	APIMessage message_reply(request_message);
	message_reply.mutable_node_register()->set_node_id(node_id);
	send(message_reply);

	log() << "Registered node #" << node_id << std::endl;

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

void APISession::NodeUnRegister(APIMessage request_message) {
	auto it = nodes.find(request_message.node_unregister().node_id());
	if(it == nodes.end()){
		remoteThrow(request_message, std::error_condition((int)P2PError::nonexistent_node, P2PErrorCategory));
		return;
	}

	p2p::NodeManager::getInstance()->destroyNode(it->second.node);
	nodes.erase(it);

	log() << "Unegistered node #" << request_message.node_unregister().node_id() << std::endl;
	if(next_node_id > request_message.node_unregister().node_id() || next_node_id == 0){
		next_node_id = request_message.node_unregister().node_id();
	}

	// Reply to client
	APIMessage reply_message(request_message);
	send(reply_message);
}

void APISession::NodeConnect(APIMessage request_message) {
	throw;
}

void APISession::NodeAccept(APIMessage request_message) {
	throw;
}

void APISession::NodeListen(APIMessage request_message) {
	auto node_it = nodes.find(request_message.node_listen().node_id());
	if(node_it == nodes.end()){
		remoteThrow(request_message, std::error_condition((int)P2PError::nonexistent_node, P2PErrorCategory));
		return;
	}
	node_it->second.node->listen(request_message.node_listen().max_connections());

	APIMessage reply_message(request_message);
	send(reply_message);
}

void APISession::NodeBind(APIMessage request_message) {
	auto node_it = nodes.find(request_message.node_bind().node_id());
	if(node_it == nodes.end()){
		remoteThrow(request_message, std::error_condition((int)P2PError::nonexistent_node, P2PErrorCategory));
		return;
	}

	if(request_message.node_bind().has_binary_private_key()){
		node_it->second.node->bind(crypto::PrivateKeyDSA::fromBinaryString(request_message.node_bind().binary_private_key()));
	}else if(request_message.node_bind().has_b58_private_key()){
		node_it->second.node->bind(crypto::PrivateKeyDSA::fromBase58(request_message.node_bind().b58_private_key()));
	}

	log() << "Bound node #" << request_message.node_bind().node_id() << std::endl;

	APIMessage reply_message(request_message);
	send(reply_message);
}

void APISession::SocketUnRegister(APIMessage request_message) {
}

void APISession::remoteThrow(APIMessage request_message, std::error_condition error){
	APIMessage reply_message;
	reply_message.mutable_error()->set_error_category(error.category().name());
	reply_message.mutable_error()->set_error_code(error.value());
	reply_message.mutable_error()->set_error_what(error.message());

	reply_message.set_type(request_message.type());
	reply_message.set_message_token(request_message.message_token());
}

void APISession::dropSession() {
	//TODO: delete nodes.
	parent_api_server->dropSession(this);
}

} /* namespace api */
} /* namespace p2pnet */
