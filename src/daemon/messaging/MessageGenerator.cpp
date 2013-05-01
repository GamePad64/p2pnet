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

#include "MessageGenerator.h"
#include "../protobuf/Protocol.pb.h"

namespace p2pnet {
namespace messaging {

MessageGenerator::MessageGenerator() {
	pks = databases::PersonalKeyStorage::getInstance();
}
MessageGenerator::~MessageGenerator() {}

protocol::p2pMessage MessageGenerator::generateMessage(peer::TH src_th, peer::TH dest_th,
		protocol::p2pMessage_Payload payload) {
	protocol::p2pMessage message;

	message.mutable_header()->set_src_th(src_th.toBinaryString());
	message.mutable_header()->set_dest_th(dest_th.toBinaryString());

	message.mutable_payload() = payload;



	return message;
}

protocol::p2pMessage MessageGenerator::generateMessage(peer::TH dest_th, protocol::p2pMessage_Payload payload) {
	return generateMessage(pks->getMyTransportHash(), dest_th, payload);
}

// Payload generators
protocol::p2pMessage_Payload MessageGenerator::generateKeyExchangeRequestPayload() {
	protocol::p2pMessage_Payload payload;
	protocol::p2pMessage_Payload_KeyExchangeRequestPart key_exchange_request;

	payload.set_message_type(protocol::p2pMessage_Payload_MessageType_KEY_EXCHANGE_REQUEST);

	key_exchange_request.set_src_pubkey(pks->getMyPublicKey().toBinaryString());
	payload.set_serialized_payload(key_exchange_request.SerializeAsString());
	return payload;
}

protocol::p2pMessage_Payload MessageGenerator::generateKeyExchangeResponsePayload() {
}

protocol::p2pMessage_Payload MessageGenerator::generateAgreementPayload() {
	//protocol::p2pMessage_Payload payload;
	//payload.set_message_type(protocol::p2p);
}

} /* namespace messaging */
} /* namespace p2pnet */
