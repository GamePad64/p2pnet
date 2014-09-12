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
#include "Protocol.pb.h"
#include "../overlay/OverlayKeyProvider.h"
#include "../transport/Connection.h"
#include "../transport/Socket.h"

namespace p2pnet {
namespace discovery {

DiscoveryService::DiscoveryService(std::shared_ptr<transport::Socket> socket) : socket(socket) {}
DiscoveryService::~DiscoveryService() {}

protocol::ConnectionRequestMessage DiscoveryService::generateConnectionRequest() {
	auto pks = overlay::OverlaySocket::getInstance()->getKeyProvider();

	protocol::ConnectionRequestMessage message;
	message.set_src_th(pks->getKeyInfo()->th.toBinaryString());
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
