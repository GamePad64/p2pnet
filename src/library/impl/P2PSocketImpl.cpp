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
#include "P2PSocketImpl.h"

namespace p2pnet {
namespace impl {

// Constructors
P2PSocketImpl::P2PSocketImpl() : P2PSocketImpl(Singleton<P2PDaemonConnectionImpl>::getInstance()) {}

P2PSocketImpl::P2PSocketImpl(P2PDaemonConnectionImpl* parent_socket_manager) {
	m_parent_socket_manager = parent_socket_manager;
	m_api_client = m_parent_socket_manager->getAPIClient();

	api::APIMessage send_message;
	send_message.set_type(send_message.REGISTER_SOCKET);

	int ec;
	m_api_client->send(send_message, ec);
	if(!ec){
		api::APIMessage recv_message;
		recv_message = m_api_client->receive(ec);
		if(recv_message.type() == recv_message.REGISTER_SOCKET_CALLBACK)
			socket_id = recv_message.socket_id();
	}else{
		//EXCEPTION
	}
}

P2PSocketImpl::P2PSocketImpl(std::string base58_private_key) : P2PSocketImpl() {
	this->bind(base58_private_key);
}

P2PSocketImpl::P2PSocketImpl(std::string base58_private_key,
		P2PDaemonConnectionImpl* parent_socket_manager) : P2PSocketImpl(parent_socket_manager) {
	this->bind(base58_private_key);
}

P2PSocketImpl::~P2PSocketImpl() {
	api::APIMessage send_message;
	send_message.set_type(send_message.UNREGISTER_SOCKET);
	send_message.set_socket_id(socket_id);

	int ec;
	m_api_client->send(send_message, ec);
	if(!ec){
		api::APIMessage recv_message;
		recv_message = m_api_client->receive(ec);
	}else{
		//EXCEPTION
	}
}

void P2PSocketImpl::bind(std::string base58_private_key) {
	api::APIMessage send_message;
	send_message.set_type(send_message.BIND_SOCKET);
	send_message.set_privkey_b58(base58_private_key);

	int ec;
	m_api_client->send(send_message, ec);
	if(!ec){
		api::APIMessage recv_message;
		recv_message = m_api_client->receive(ec);
		if(recv_message.type() == recv_message.BIND_SOCKET_CALLBACK)
			m_bound_private_key.setAsBase58(base58_private_key);
	}else{
		//EXCEPTION
	}
}

P2PSocketImpl* P2PSocketImpl::accept() {
}

void P2PSocketImpl::connect(std::string SH) {
}

void P2PSocketImpl::disconnect() {
}

void P2PSocketImpl::listen() {
}

P2PContextImpl* P2PSocketImpl::createContext(P2PContextImpl::ContextType type) {
}

P2PContextImpl* P2PSocketImpl::acceptContext(P2PContextImpl::ContextType type) {
}

void P2PSocketImpl::closeContext(P2PContextImpl* context_ptr) {
}

void P2PSocketImpl::closeContext(uint32_t context_number) {
}

} /* namespace impl */
} /* namespace p2pnet */
