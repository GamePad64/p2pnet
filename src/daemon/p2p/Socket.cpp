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
#include "Socket.h"

#include "Node.h"
#include "NodeManager.h"
#include "LoopbackConnection.h"

namespace p2pnet {
namespace p2p {

Socket::Socket(Node* node, Connection* incoming_connections){
	parent_node = node;
}

Socket::Socket(Node* node, SH sh_to_connect){
	parent_node = node;

	// Loopback implementation
	if(parent_node->loopback()){
		auto other_node = NodeManager::getInstance()->getBoundNode(sh_to_connect);
		if(other_node != nullptr && other_node->loopback()){
			connection = new LoopbackConnection();
			return;
		}else{

		}
	}

	// Remote implementation
}

Socket::~Socket(){

}

SH Socket::getRemoteSH() {
	return connection->getRemoteSH();
}

}
/* namespace p2p */
} /* namespace p2pnet */
