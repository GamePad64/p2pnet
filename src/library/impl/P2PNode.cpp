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

#include "../../common/Singleton.h"

#include "Impl.h"

namespace p2pnet {

// Constructors
P2PNode::P2PNode() : P2PNode(SharedSingleton<P2PLocalDaemon>::getInstance()) {}

P2PNode::P2PNode(P2PDaemon* parent_daemon) {
	impl = new Impl();
	Loggable::log("P2PNode") << "Registering new node." << std::endl;
	impl->m_parent_daemon_ptr = parent_daemon;

	// Generating request
	api::APIMessage send_message;

	api::APIMessage::NodeRegister send_api_message;

	send_message.set_type(send_message.NODE_REGISTER);
	send_message.set_api_message(send_api_message.SerializeAsString());
	// End of request

	int ec;
	impl->m_parent_daemon_ptr->send(send_message, ec);
	if(!ec){
		api::APIMessage recv_message;
		recv_message = impl->m_parent_daemon_ptr->receive(ec);
		api::APIMessage::NodeRegisterCallback api_message;
		api_message.ParseFromString(recv_message.api_message());

		if(recv_message.type() == recv_message.NODE_REGISTER_CALLBACK){
			impl->node_id = api_message.node_id();
			Loggable::log("P2PNode") << "Registered node: #" << impl->node_id << std::endl;
			if(impl->node_id == 0){
				// Ouch, bad sign. It means, that p2pnetd didn't create the node, as there are too much of them;
				// TODO: new exception model.
				throw std::out_of_range("Node count exceeded maximum");
			}
		}
	}else{
		// TODO: new exception model.
		// TODO: make this async
		throw std::runtime_error("p2pnetd connection problem");
	}
}

P2PNode::P2PNode(std::shared_ptr<P2PDaemon> parent_shared_daemon) : P2PNode(parent_shared_daemon.get()) {
	impl->m_parent_daemon_shared = parent_shared_daemon;
}

P2PNode::~P2PNode() {
	// Generating request
	api::APIMessage send_message;

	api::APIMessage::NodeUnRegister send_api_message;
	send_api_message.set_node_id(impl->node_id);

	send_message.set_type(send_message.NODE_UNREGISTER);
	send_message.set_api_message(send_api_message.SerializeAsString());
	// End of request

	int ec;
	impl->m_parent_daemon_ptr->send(send_message, ec);
	if(!ec){
		Loggable::log("P2PSocket") << "Unregistering node: #" << impl->node_id << std::endl;
		auto recv_message = impl->m_parent_daemon_ptr->receive(ec);
		delete impl;
	}else{
		// TODO: new exception model.
		throw std::runtime_error("p2pnetd connection problem");
	}
}

void P2PNode::bind(std::string base58_private_key) {
	// Generating request
	api::APIMessage send_message;

	api::APIMessage::NodeBind send_api_message;
	send_api_message.set_node_id(impl->node_id);
	send_api_message.set_b58_private_key(base58_private_key);

	send_message.set_type(send_message.NODE_BIND);
	send_message.set_api_message(send_api_message.SerializeAsString());
	// End of request

	int ec;
	impl->m_parent_daemon_ptr->send(send_message, ec);
	if(!ec){
		api::APIMessage recv_message;
		recv_message = impl->m_parent_daemon_ptr->receive(ec);
		if(recv_message.type() == recv_message.NODE_BIND_CALLBACK)
			impl->m_bound_private_key.setAsBase58(base58_private_key);
	}else{
		// TODO: new exception model.
		throw std::runtime_error("p2pnetd connection problem");
	}
}

P2PSocket* P2PNode::accept() {
}

P2PSocket* P2PNode::connect(std::string SH) {
}

void P2PNode::listen(uint32_t max_conn) {
	// Generating request
	api::APIMessage send_message;

	api::APIMessage::NodeListen send_api_message;
	send_api_message.set_node_id(impl->node_id);
	send_api_message.set_max_connections(max_conn);

	send_message.set_type(send_message.NODE_LISTEN);
	send_message.set_api_message(send_api_message.SerializeAsString());
	// End of request

	int ec;
	impl->m_parent_daemon_ptr->send(send_message, ec);
	if(!ec){
		api::APIMessage recv_message;
		recv_message = impl->m_parent_daemon_ptr->receive(ec);
		if(recv_message.type() == recv_message.NODE_LISTEN_CALLBACK){}
	}else{
		// TODO: new exception model.
		throw std::runtime_error("p2pnetd connection problem");
	}
}

P2PNode* p2p_createNode() {	// TODO: Really stub
	try {
		auto new_socket = new P2PNode();
		return new_socket;
	}catch(std::exception& e){
		std::cout << e.what() << std::endl;
		return 0;
	}
	return 0;
}

P2PNode* p2p_createNodeOnDaemon(P2PDaemon* parent_socket_manager) {	// TODO: Really stub
	try {
		auto new_socket = new P2PNode(parent_socket_manager);
		return new_socket;
	}catch(std::exception& e){
		std::cout << e.what() << std::endl;
		return 0;
	}
	return 0;
}

void p2p_destroyNode(P2PNode* socket) {
	delete socket;
}

void p2p_bindSocket(P2PNode* socket, char* base58_private_key) {
	try {
		socket->bind(std::string(base58_private_key));
	}catch(std::exception& e){
		std::cout << e.what() << std::endl;
	}
}

void p2p_listenSocket(P2PNode* socket, uint32_t max_conn) {
	try {
		socket->listen(max_conn);
	}catch(std::exception& e){
		std::cout << e.what() << std::endl;
	}
}

} /* namespace p2pnet */
