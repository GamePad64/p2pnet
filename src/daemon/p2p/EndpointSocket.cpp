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
#include "EndpointSocket.h"

#include "EndpointManager.h"

namespace p2pnet {
namespace endpoint {

EndpointSocket::EndpointSocket(api::APISession* parent) : parent_api_session(parent), connected_endpoint(nullptr) {}

EndpointSocket::~EndpointSocket() {}

void EndpointSocket::bind(crypto::PrivateKeyDSA privkey) {
	bound_privkey = privkey;
	EndpointManager::getInstance()->setBound(shared_from_this());
	log() << "Socket bound to: " << privkey.toBase58() << std::endl;
}

void EndpointSocket::listen(int max_conn) {
	listen_max_conn = max_conn;
	log() << "Socket set backlog to: " << max_conn << std::endl;
}

void EndpointSocket::connect(SH destination_sh) {
	auto destination_ptr = EndpointManager::getInstance()->getLoopbackDestination(destination_sh);
	if(destination_ptr){
		destination_ptr->handleConnectRequest(shared_from_this());
	}
}

void EndpointSocket::accept() {

}

SH EndpointSocket::getBoundSH() {
	auto public_key = bound_privkey.derivePublicKey();
	return SH(public_key);
}

void EndpointSocket::handleConnectRequest(std::shared_ptr<GenericDestination> endpoint) {
	if(accept_queue.size() < listen_max_conn) {
		accept_queue.push_back(endpoint.get());
		log() << "Socket accept queue length: " << accept_queue.size() << std::endl;
	}else{
		// TODO: send "can't connect".
		log() << "Max connections exceeded" << std::endl;
	}
}

} /* namespace endpoint */
} /* namespace p2pnet */
