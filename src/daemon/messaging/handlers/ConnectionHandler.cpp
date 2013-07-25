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

#include "ConnectionHandler.h"

namespace p2pnet {
namespace messaging {
namespace handlers {

ConnectionHandler::ConnectionHandler(MessageSocket* socket_ptr) :
		MessageHandler(socket_ptr) {
	m_netdb_storage = databases::NetDBStorage::getInstance();
}
ConnectionHandler::~ConnectionHandler() {
}

std::string ConnectionHandler::getHandlerName() {
	return "ConnectionHandler";
}

void ConnectionHandler::processReceivedMessage(protocol::p2pMessage& message, MessageState& message_state,
		Session::pointer session_ptr) {
	protocol::p2pMessage_Payload payload = message.payload();
	if (payload.message_type() == payload.CONNECTION) {
		// So, this is payload from the message.
		std::string original_payload = payload.serialized_payload();
		/*
		 * Now we need to decrypt this message. CONNECTION message is always encrypted using
		 * my public key (that, from PersonalKeyStorage).
		 */
		std::string decrypted_payload = databases::PersonalKeyStorage::getInstance()->getMyPrivateKey().decrypt(
				original_payload);
		/*
		 * And now we must parse decrypted payload.
		 */
		protocol::p2pMessage_Payload_ConnectionPart deserialized_payload;
		if(deserialized_payload.ParseFromString(decrypted_payload))
			reject(Reason::PARSE_ERROR);

		/*
		 * Now we know, that protobuf message parsed correctly and message format is correct.
		 */
		auto remote_ecdsa_pubkey = crypto::PublicKeyDSA::fromBinaryString(deserialized_payload.src_ecdsa_pubkey());
		if (!remote_ecdsa_pubkey.validate()) reject(Reason::KEY_INVALID, "Public key is inconsistent");
		if (crypto::Hash::compute(deserialized_payload.src_ecdsa_pubkey()).toBinaryString()
				!= message.header().src_th()) reject(Reason::KEY_INVALID, "Hashed Public Key is inconsistent with TH");


		bool response = deserialized_payload.has_src_ecdsa_pubkey();
		// This generates text string used to sign this message.
		std::string text_to_sign = response ?
						deserialized_payload.src_ecdsa_pubkey() + deserialized_payload.src_ecdh_pubkey() :
						deserialized_payload.src_ecdh_pubkey();
		if (!remote_ecdsa_pubkey.verify(text_to_sign, deserialized_payload.signature()))
			reject(Reason::KEY_INVALID, "Signature is invalid");

		/*
		 * So, situation:
		 * We know that message was encrypted using our key, because
		 * - it is parsed correctly
		 * - sender's TH is a real hash of public key defined in header
		 * - sender's public key is consistent (I mean, it is a valid ECDSA public key in DER format)
		 *
		 * We have:
		 * - sender's public key (if it is not a response)
		 * - Session ECDH key, so now we can generate a symmetric key.
		 */
		std::clog << "[" << getHandlerName() << "] Received \"Connection\" message from: TH:" << crypto::Hash::fromBinaryString(message.header().src_th()).toBase58() << std::endl;
	}
}

void ConnectionHandler::processSentMessage(protocol::p2pMessage& message, MessageState& message_state,
		Session::pointer session_ptr) {
	protocol::p2pMessage_Payload payload = message.payload();
	if (payload.message_type() == payload.CONNECTION) {
		std::clog << "[" << getHandlerName() << "] Sent Connection message to "
				<< peer::TH::fromBinaryString(message.header().dest_th()).toBase58() << std::endl;
	}
}

} /* namespace handlers */
} /* namespace messaging */
} /* namespace p2pnet */
