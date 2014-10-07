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

#include "DiscoveryService.h"
#include "../overlay/KeyProvider.h"
#include "../transport/Connection.h"
#include "../transport/Socket.h"

namespace p2pnet {
namespace discovery {

DiscoveryService::DiscoveryService(std::shared_ptr<transport::Socket> transport_socket, std::shared_ptr<overlay::Socket> overlay_socket) :
		transport_socket(transport_socket), overlay_socket(overlay_socket) {}
DiscoveryService::~DiscoveryService() {}

protocol::OverlayMessage DiscoveryService::generateConnectionRequest() {
	auto pks = overlay_socket->getKeyProvider();

	protocol::OverlayMessage message;
	message.mutable_header()->set_src_th(pks->getKeyInfo()->th.toBinaryString());
	return message;
}

void DiscoveryService::handshake(transport::SocketEndpoint endpoint) {
	if(socket->getConnection(endpoint) == nullptr){
		socket->connect(endpoint, )
		transport_socket->send(endpoint, generateConnectionRequest().SerializeAsString());
	}
}

} /* namespace discovery */
} /* namespace p2pnet */
