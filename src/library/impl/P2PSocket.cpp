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

namespace p2pnet {

class P2PSocket::Impl {
public:
	std::shared_ptr<P2PDaemon> m_parent_daemon;

	uint32_t socket_id;
	std::map<uint32_t, P2PContext*> m_contexts;

	crypto::PrivateKeyDSA m_bound_private_key;
	crypto::PrivateKeyDSA m_connected_private_key;
	std::string m_remote_sh;
};

// Constructors
P2PSocket::P2PSocket() : P2PSocket(SharedSingleton<P2PLocalDaemon>::getInstance()) {}

P2PSocket::P2PSocket(P2PDaemon* parent_daemon) : P2PSocket(std::shared_ptr<P2PDaemon>(parent_daemon)) {}

P2PSocket::P2PSocket(std::shared_ptr<P2PDaemon> parent_shared_daemon) {
	impl = new Impl();
	Loggable::log("P2PSocket") << "Registering new socket." << std::endl;
	impl->m_parent_daemon = parent_shared_daemon;

	api::APIMessage send_message;
	send_message.set_type(send_message.REGISTER_SOCKET);

	int ec;
	impl->m_parent_daemon->send(send_message, ec);
	if(!ec){
		api::APIMessage recv_message;
		recv_message = impl->m_parent_daemon->receive(ec);
		if(recv_message.type() == recv_message.REGISTER_SOCKET_CALLBACK){
			impl->socket_id = recv_message.socket_id();
			Loggable::log("P2PSocket") << "Registered socket: #" << impl->socket_id << std::endl;
			if(impl->socket_id == 0){
				// Ouch, bad sign. It means, that p2pnetd didn't create the socket, as there are too much of them;
				// TODO: new exception model.
				throw std::out_of_range("Socket count exceeded maximum");
			}
		}
	}else{
		// TODO: new exception model.
		throw std::runtime_error("p2pnetd connection problem");
	}
}

P2PSocket::P2PSocket(std::string base58_private_key) : P2PSocket() {
	this->bind(base58_private_key);
	delete impl;
}

P2PSocket::P2PSocket(std::string base58_private_key,
		P2PDaemon* parent_daemon) : P2PSocket(parent_daemon) {
	this->bind(base58_private_key);
}

P2PSocket::~P2PSocket() {
	api::APIMessage send_message;
	send_message.set_type(send_message.UNREGISTER_SOCKET);
	send_message.set_socket_id(impl->socket_id);

	int ec;
	impl->m_parent_daemon->send(send_message, ec);
	if(!ec){
		Loggable::log("P2PSocket") << "Unregistering socket: #" << impl->socket_id << std::endl;
		auto recv_message = impl->m_parent_daemon->receive(ec);
		delete impl;
	}else{
		// TODO: new exception model.
		throw std::runtime_error("p2pnetd connection problem");
	}
}

void P2PSocket::bind(std::string base58_private_key) {
	api::APIMessage send_message;
	send_message.set_type(send_message.BIND_SOCKET);
	send_message.set_socket_id(impl->socket_id);
	send_message.set_privkey_b58(base58_private_key);

	int ec;
	impl->m_parent_daemon->send(send_message, ec);
	if(!ec){
		api::APIMessage recv_message;
		recv_message = impl->m_parent_daemon->receive(ec);
		if(recv_message.type() == recv_message.BIND_SOCKET_CALLBACK)
			impl->m_bound_private_key.setAsBase58(base58_private_key);
	}else{
		// TODO: new exception model.
		throw std::runtime_error("p2pnetd connection problem");
	}
}

P2PSocket* P2PSocket::accept() {
}

void P2PSocket::connect(std::string SH) {
}

void P2PSocket::disconnect() {
}

void P2PSocket::listen(uint32_t max_conn) {
	api::APIMessage send_message;
	send_message.set_type(send_message.LISTEN);
	send_message.set_socket_id(impl->socket_id);
	send_message.set_max_connections(max_conn);

	int ec;
	impl->m_parent_daemon->send(send_message, ec);
	if(!ec){
		api::APIMessage recv_message;
		recv_message = impl->m_parent_daemon->receive(ec);
		if(recv_message.type() == recv_message.LISTEN_CALLBACK){}
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

P2PSocket* p2p_createSocket() {	// TODO: Really stub
	try {
		auto new_socket = new P2PSocket();
		return new_socket;
	}catch(std::exception& e){
		std::cout << e.what() << std::endl;
		return 0;
	}
	return 0;
}

P2PSocket* p2p_createSocketOnDaemon(P2PDaemon* parent_socket_manager) {	// TODO: Really stub
	try {
		auto new_socket = new P2PSocket(parent_socket_manager);
		return new_socket;
	}catch(std::exception& e){
		std::cout << e.what() << std::endl;
		return 0;
	}
	return 0;
}

void p2p_destroySocket(P2PSocket* socket) {
	delete socket;
}

void p2p_bindSocket(P2PSocket* socket, char* base58_private_key) {
	try {
		socket->bind(std::string(base58_private_key));
	}catch(std::exception& e){
		std::cout << e.what() << std::endl;
	}
}

void p2p_listenSocket(P2PSocket* socket, uint32_t max_conn) {
	try {
		socket->listen(max_conn);
	}catch(std::exception& e){
		std::cout << e.what() << std::endl;
	}
}

} /* namespace p2pnet */
