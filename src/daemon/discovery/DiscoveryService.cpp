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
#include "../protobuf/Protocol.pb.h"
#include "../databases/PersonalKeyStorage.h"
#include "../transport/TransportConnection.h"
#include "../transport/TransportSocket.h"

namespace p2pnet {
namespace discovery {

DiscoveryService::DiscoveryService() {}
DiscoveryService::~DiscoveryService() {}

void DiscoveryService::handshake(transport::TransportSocketEndpoint endpoint) {
	auto transport_socket = transport::TransportSocket::getInstance();

	protocol::OverlayMessageStructure message;
	message.mutable_header()->set_src_th(databases::PersonalKeyStorage::getInstance()->getMyTransportHash().toBinaryString());
	message.mutable_payload()->set_message_type(message.payload().CONNECTION_PUBKEY);

	protocol::OverlayMessageStructure_Payload_ConnectionPart payload_s;
	payload_s.set_src_ecdsa_pubkey(databases::PersonalKeyStorage::getInstance()->getMyPublicKey().toBinaryString());

	message.mutable_payload()->set_serialized_payload(payload_s.SerializeAsString());

	transport_socket->send(endpoint, message.SerializeAsString());
}

} /* namespace discovery */
} /* namespace p2pnet */
