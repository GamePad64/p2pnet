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

#include "../../common/crypto/PrivateKeyDSA.h"
#include "../../common/Singleton.h"

#include "Impl.h"

namespace p2pnet {

// Constructors
P2PSocket::P2PSocket(P2PNode* parent_node) {
	impl = new Impl;

	impl->m_parent_node = parent_node;
	impl->node_id = parent_node->impl->node_id;
	impl->m_parent_daemon_ptr = parent_node->impl->m_parent_daemon_ptr;

	Loggable::log("P2PNode") << "Registering new socket." << std::endl;

	// Generating request
	api::APIMessage send_message;

	api::APIMessage::SocketRegister send_api_message;
	send_api_message.set_node_id(impl->node_id);

	send_message.set_type(send_message.SOCKET_REGISTER);
	send_message.set_api_message(send_api_message.SerializeAsString());
	// End of request

	int ec;
	impl->m_parent_daemon_ptr->send(send_message, ec);
	if(!ec){
		api::APIMessage recv_message;
		recv_message = impl->m_parent_daemon_ptr->receive(ec);
		api::APIMessage::SocketRegisterCallback api_message;
		api_message.ParseFromString(recv_message.api_message());

		if(recv_message.type() == recv_message.SOCKET_REGISTER_CALLBACK){
			impl->socket_id = api_message.node_id();
			Loggable::log("P2PNode") << "Registered socket: #" << impl->node_id << std::endl;
			if(impl->socket_id == 0){
				// Ouch, bad sign. It means, that p2pnetd didn't create the node, as there are too much of them;
				// TODO: new exception model.
				throw std::out_of_range("Socket count exceeded maximum");
			}
		}
	}else{
		// TODO: new exception model.
		// TODO: make this async
		throw std::runtime_error("p2pnetd connection problem");
	}
}

P2PSocket::~P2PSocket() {
	// Generating request
	api::APIMessage send_message;

	api::APIMessage::SocketUnRegister send_api_message;
	send_api_message.set_node_id(impl->node_id);
	send_api_message.set_socket_id(impl->node_id);

	send_message.set_type(send_message.SOCKET_UNREGISTER);
	send_message.set_api_message(send_api_message.SerializeAsString());
	// End of request

	int ec;
	impl->m_parent_daemon_ptr->send(send_message, ec);
	if(!ec){
		Loggable::log("P2PSocket") << "Unregistering socket: #" << impl->socket_id << std::endl;
		auto recv_message = impl->m_parent_daemon_ptr->receive(ec);
		delete impl;
	}else{
		// TODO: new exception model.
		throw std::runtime_error("p2pnetd connection problem");
	}
}

P2PContext* P2PSocket::createContext(ContextType type) {
}

P2PContext* P2PSocket::acceptContext(ContextType type) {
}

void P2PSocket::closeContext(P2PContext* context_ptr) {
}

void P2PSocket::closeContext(uint32_t context_number) {
}

void p2p_destroySocket(P2PSocket* socket) {
	delete socket;
}


} /* namespace p2pnet */
