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
#include "../../common/crypto/crc32.h"

namespace p2pnet {
namespace messaging {

MessageGenerator::MessageGenerator() {
	pks = databases::PersonalKeyStorage::getInstance();
}
MessageGenerator::~MessageGenerator() {}

std::string MessageGenerator::prepareForCRC32(const protocol::p2pMessage& message) {
	return message.header().SerializeAsString() + message.payload().SerializeAsString();
}

bool MessageGenerator::checkMessageCRC32(protocol::p2pMessage message){
	auto crc32_real = crypto::computeCRC32(prepareForCRC32(message));
	auto crc32_message = message.crc32();
	return (crc32_real == crc32_message);
}

protocol::p2pMessage MessageGenerator::generateMessage(const protocol::p2pMessage_Header& header,
		const protocol::p2pMessage_Payload& payload) {
	protocol::p2pMessage message;

	// Step I: Inserting header into message.
	*(message.mutable_header()) = header;
	// Step II: Inserting payload into message.
	*(message.mutable_payload()) = payload;
	// Step III: Concatenating header with payload and computing CRC-32.
	//           Maybe, it will be CRC-32C in next releases, because it is CPU-accelerated in SSE4.2.
	message.set_crc32(crypto::computeCRC32(prepareForCRC32(message)));

	return message;
}

protocol::p2pMessage MessageGenerator::generateMessage(const peer::TH& src_th,
		const peer::TH& dest_th,
		const protocol::p2pMessage_Payload& payload) {
	// Step N: Generating header, which consists of source end destination.
	protocol::p2pMessage_Header header;
	header.set_src_th(src_th.toBinaryString());
	header.set_dest_th(dest_th.toBinaryString());

	return generateMessage(header, payload);
}

protocol::p2pMessage MessageGenerator::generateMessage(const peer::TH& dest_th,
		const protocol::p2pMessage_Payload& payload) {
	return generateMessage(pks->getMyTransportHash(), dest_th, payload);
}

// Payload generators
protocol::p2pMessage_Payload MessageGenerator::generateKeyExchangePayload() {
	protocol::p2pMessage_Payload payload;
	protocol::p2pMessage_Payload_KeyRequestPart part;

	payload.set_message_type(payload.KEY_REQUEST);

	part.set_src_pubkey(pks->getMyPublicKey().toBinaryString());
	part.set_signature(pks->getMyPrivateKey().sign(pks->getMyPublicKey().toBinaryString()));// OPTIMIZE: cache this message, or we will sign new message every time we receive request.

	payload.set_serialized_payload(part.SerializeAsString());
	return payload;
}

protocol::p2pMessage_Payload MessageGenerator::generateConnectionPayload(std::string ecdh_pubkey, bool response) {
	protocol::p2pMessage_Payload payload;
	protocol::p2pMessage_Payload_ConnectionPart part;

	payload.set_message_type(payload.CONNECTION);

	auto my_ecdsa_private_key = pks->getMyPrivateKey();

	part.set_src_ecdh_pubkey(ecdh_pubkey);

	if(!response){
		part.set_src_ecdsa_pubkey(my_ecdsa_private_key.derivePublicKey().toBinaryString());
		part.set_signature(my_ecdsa_private_key.sign( (part.src_ecdsa_pubkey()) + (part.src_ecdh_pubkey()) ));
	}else{
		part.set_signature(my_ecdsa_private_key.sign( part.src_ecdh_pubkey() ));
	}

	payload.set_serialized_payload(part.SerializeAsString());

	return payload;
}

} /* namespace messaging */
} /* namespace p2pnet */
