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
#include "EndpointManager.h"

#include "EndpointSocket.h"

namespace p2pnet {
namespace endpoint {

EndpointManager::EndpointManager() {}
EndpointManager::~EndpointManager() {}

std::shared_ptr<GenericDestination> EndpointManager::getLoopbackDestination(SH sh) {
	if(getValue<bool>("endpoint.allow_loopback")){
		auto endp_it = bound_local_sockets.find(sh);
		return (endp_it == bound_local_sockets.end()) ? nullptr : endp_it->second;
	}
	return nullptr;
}

void EndpointManager::setBound(std::shared_ptr< EndpointSocket > endpoint_socket_ptr) {
	unbound_local_sockets.erase(endpoint_socket_ptr);
	bound_local_sockets.insert(std::make_pair(endpoint_socket_ptr->getBoundSH(), endpoint_socket_ptr));
}

void EndpointManager::setUnbound(std::shared_ptr< EndpointSocket > endpoint_socket_ptr) {
	bound_local_sockets.erase(endpoint_socket_ptr->getBoundSH());
	unbound_local_sockets.insert(endpoint_socket_ptr);
}

void EndpointManager::registerSocket(std::shared_ptr< EndpointSocket > endpoint_socket_ptr) {
	unbound_local_sockets.insert(endpoint_socket_ptr);
}

void EndpointManager::unregisterSocket(std::shared_ptr< EndpointSocket > endpoint_socket_ptr) {
	if(unbound_local_sockets.erase(endpoint_socket_ptr) != 1){
		bound_local_sockets.erase(endpoint_socket_ptr->getBoundSH());
	}
}

} /* namespace endpoint */
} /* namespace p2pnet */
