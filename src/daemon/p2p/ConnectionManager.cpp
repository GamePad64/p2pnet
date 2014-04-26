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
#include "ConnectionManager.h"

namespace p2pnet {
namespace p2p {

ConnectionManager::ConnectionManager() {}
ConnectionManager::~ConnectionManager() {
	for(auto bound_it : bound_sockets){
		delete bound_it.second;
	}
	for(auto not_bound_it : not_bound_sockets){
		delete not_bound_it;
	}
}

LocalSocket* ConnectionManager::createSocket() {
	auto new_local_socket = new LocalSocket();
	not_bound_sockets.insert(new_local_socket);
	return new_local_socket;
}

LocalSocket* ConnectionManager::createSocket(api::APISession* api_session) {
	auto new_local_socket = new LocalSocket(api_session);
	not_bound_sockets.insert(new_local_socket);
	return new_local_socket;
}

void ConnectionManager::destroySocket(LocalSocket* socket_ptr) {
	if(socket_ptr->isBound()){
		auto bound_it = bound_sockets.find(socket_ptr->getLocalSH());
		bound_sockets.erase(bound_it);
	}else{
		not_bound_sockets.erase(socket_ptr);
	}
}

} /* namespace p2p */
} /* namespace p2pnet */
