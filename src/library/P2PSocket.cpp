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

#include "../common/crypto/PrivateKeyDSA.h"
#include "../common/Singleton.h"

#include "Impl.h"

namespace p2pnet {

// Constructors
P2PSocket::P2PSocket(P2PNode* parent_node, uint32_t socket_id) {	// This constructor is passive. It is called only on P2PNode.connect() or P2PNode.accept()
	impl = new Impl;

	impl->m_parent_node = parent_node;
	impl->parent_daemon = parent_node->impl->parent_daemon;
	impl->node_id = parent_node->impl->node_id;
	impl->socket_id = socket_id;
}

P2PSocket::~P2PSocket() {
	// Generating request
	api::APIMessage message_request;
	message_request.set_type(message_request.SOCKET_UNREGISTER);
	message_request.mutable_socket_unregister()->set_node_id(impl->node_id);
	message_request.mutable_socket_unregister()->set_socket_id(impl->socket_id);
	// End of request

	Loggable::log("P2PSocket") << "Unregistering socket: #" << impl->node_id << "." << impl->socket_id << std::endl;
	try {
		impl->parent_daemon->clientExchange(message_request);
	} catch (std::system_error& e){
		Loggable::log("P2PSocket") << "Socket #" << impl->node_id << "." << impl->socket_id << " not destructed properly. Exception: " << e.what() << std::endl;
	}

	delete impl;
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
