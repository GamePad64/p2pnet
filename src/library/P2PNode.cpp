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
#include "../p2pnet.h"
#include "daemon_conn/P2PLocalDaemon.h"

#include "../common/Singleton.h"

#include "Impl.h"

namespace p2pnet {

// Constructors
P2PNode::P2PNode() : P2PNode(SharedSingleton<P2PLocalDaemon>::getInstance()) {}

P2PNode::P2PNode(P2PDaemon* parent_daemon) {
	impl = new Impl();
	Loggable::log("P2PNode") << "Registering new node." << std::endl;
	impl->parent_daemon = parent_daemon;

	// Generating request
	api::APIMessage message_request;
	message_request.set_type(message_request.NODE_REGISTER);
	// End of request

	api::APIMessage message_reply;
	try {
		message_reply = impl->parent_daemon->clientExchange(message_request);
	} catch(std::system_error& e){
		delete impl;
		throw;
	}

	impl->node_id = message_reply.node_register().node_id();
	Loggable::log("P2PNode") << "Registered node: #" << impl->node_id << std::endl;
}

P2PNode::P2PNode(std::shared_ptr<P2PDaemon> parent_shared_daemon) : P2PNode(parent_shared_daemon.get()) {
	impl->m_parent_daemon_shared = parent_shared_daemon;
}

P2PNode::~P2PNode() {
	// Generating request
	api::APIMessage message_request;
	message_request.set_type(message_request.NODE_UNREGISTER);
	message_request.mutable_node_unregister()->set_node_id(impl->node_id);
	// End of request

	Loggable::log("P2PSocket") << "Unregistering node: #" << impl->node_id << std::endl;
	try {
		impl->parent_daemon->clientExchange(message_request);
	} catch (std::system_error& e){
		Loggable::log("P2PSocket") << "Node #" << impl->node_id << " not destructed properly. Exception: " << e.what() << std::endl;
	}

	delete impl;
}

void P2PNode::bind(std::string base58_private_key) {
	// Generating request
	api::APIMessage message_request;
	message_request.set_type(message_request.NODE_BIND);

	message_request.mutable_node_bind()->set_node_id(impl->node_id);
	message_request.mutable_node_bind()->set_b58_private_key(base58_private_key);
	// End of request

	impl->parent_daemon->clientExchange(message_request);
	impl->m_bound_private_key.setAsBase58(base58_private_key);
}

P2PSocket* P2PNode::accept() {
	// Generating request
	api::APIMessage message_request;
	message_request.set_type(message_request.NODE_ACCEPT);

	message_request.mutable_node_accept()->set_node_id(impl->node_id);
	// End of request

	auto message_reply = impl->parent_daemon->clientExchange(message_request);

	auto new_socket = new P2PSocket(this, message_reply.node_accept().node_id());
	new_socket->impl->m_remote_sh = message_reply.node_accept().sh();

	return new_socket;
}

P2PSocket* P2PNode::connect(std::string SH) {
	// Generating request
	api::APIMessage message_request;
	message_request.set_type(message_request.NODE_CONNECT);

	message_request.mutable_node_connect()->set_node_id(impl->node_id);
	message_request.mutable_node_connect()->set_sh(SH);
	// End of request

	auto message_reply = impl->parent_daemon->clientExchange(message_request);

	auto new_socket = new P2PSocket(this, message_reply.node_accept().node_id());
	new_socket->impl->m_remote_sh = message_reply.node_accept().sh();

	return new_socket;
}

void P2PNode::listen(uint32_t max_conn) {
	// Generating request
	api::APIMessage message_request;
	message_request.set_type(message_request.NODE_LISTEN);

	message_request.mutable_node_listen()->set_node_id(impl->node_id);
	message_request.mutable_node_listen()->set_max_connections(max_conn);
	// End of request

	impl->parent_daemon->clientExchange(message_request);
}

int p2p_createNode(P2PNode* node) {
	try {
		node = new P2PNode();
		return (int)P2PError::success;
	}catch(std::system_error& e){
		return e.code().value();
	}
}

int p2p_createNodeOnDaemon(P2PNode* node, P2PDaemon* parent_daemon) {	// TODO: Really stub
	try {
		node = new P2PNode(parent_daemon);
		return (int)P2PError::success;
	}catch(std::system_error& e){
		return e.code().value();
	}
}

void p2p_destroyNode(P2PNode* node) {
	delete node;
}

int p2p_accept(P2PSocket* socket, P2PNode* listening_node) {
	try {
		socket = listening_node->accept();
		return (int)P2PError::success;
	}catch(std::system_error& e){
		return e.code().value();
	}
}

int p2p_connect(P2PSocket* socket, P2PNode* connecting_node, char* SH, size_t SH_length) {
	try {
		socket = connecting_node->connect(std::string(SH, SH_length));
		return (int)P2PError::success;
	}catch(std::system_error& e){
		return e.code().value();
	}
}

int p2p_bindNode(P2PNode* node, char* base58_private_key) {
	try {
		node->bind(std::string(base58_private_key));
		return (int)P2PError::success;
	}catch(std::system_error& e){
		return e.code().value();
	}
}

int p2p_listenNode(P2PNode* node, uint32_t max_conn) {
	try {
		node->listen(max_conn);
		return (int)P2PError::success;
	}catch(std::system_error& e){
		return e.code().value();
	}
}

} /* namespace p2pnet */
