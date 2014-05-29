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
#include "Node.h"
#include "NodeManager.h"
#include "../../common/crypto/PublicKeyDSA.h"

#include <boost/none.hpp>

namespace p2pnet {
namespace p2p {

Node::Node() {
	api_session = nullptr;
}
Node::Node(api::APISession* api_session) : Node() {
	this->api_session = api_session;
}
Node::~Node() {}

SH Node::getLocalSH() {
	return bound_sh;
}

bool Node::isBound() {
	return bool(bound_sh);
}

void Node::bind(crypto::PrivateKeyDSA private_key) {
	bound_private_key = private_key;
	bound_sh = SH(private_key.derivePublicKey());
	NodeManager::getInstance()->bindNode(this, bound_sh);
}

void Node::listen(uint32_t max_connections) {
	this->max_connections = max_connections;
}

void Node::connect(SH sh) {
	Socket* connecting_socket = new Socket(this, sh);
}

void Node::accept() {
	Socket* accepting_socket = new Socket(this, incoming_connections.front());
	incoming_connections.pop_front();
	connected_sockets.insert(std::make_pair(accepting_socket->getRemoteSH(), accepting_socket));
}

bool Node::loopback() {
	return loopback_param.get_value_or(getValue<bool>("p2p.allow_loopback"));
}

void Node::loopback(bool set_loopback) {
	loopback_param = set_loopback;
}

void Node::resetLoopback() {
	loopback_param = boost::none;
}

} /* namespace p2p */
} /* namespace p2pnet */
