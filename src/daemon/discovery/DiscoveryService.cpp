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

protocol::OverlayMessageStructure DiscoveryService::generateHandshakeMessage() {
	auto pks = databases::PersonalKeyStorage::getInstance();

	protocol::OverlayMessageStructure message;
	message.mutable_header()->set_src_th(pks->getMyTransportHash().toBinaryString());
	// This is realtime message, which is sent directly to TransportSocket.
	// This is non-standard way to send messages.
	message.mutable_header()->set_prio(message.mutable_header()->REALTIME);

	auto payload_part_ptr = message.mutable_payload()->add_payload_parts();
	payload_part_ptr->set_payload_type(payload_part_ptr->CONNECTION_PUBKEY);

	protocol::OverlayMessageStructure_Payload_Part_ConnectionPart part;
	part.set_src_ecdsa_pubkey(pks->getMyPublicKey().toBinaryString());

	payload_part_ptr->set_serialized_part(part.SerializeAsString());
	return message;
}

void DiscoveryService::handshake(transport::TransportSocketEndpoint endpoint) {
	auto transport_socket = transport::TransportSocket::getInstance();

	transport_socket->send(endpoint, generateHandshakeMessage().SerializeAsString());
}

} /* namespace discovery */
} /* namespace p2pnet */
