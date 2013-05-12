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

#include "KeyExchangeHandler.h"

namespace p2pnet {
namespace messaging {
namespace handlers {

KeyExchangeHandler::KeyExchangeHandler(MessageSocket* socket_ptr) : MessageHandler(socket_ptr) {
	m_netdb_storage = databases::NetDBStorage::getInstance();
}
KeyExchangeHandler::~KeyExchangeHandler() {}

std::string KeyExchangeHandler::getHandlerName() {
	return "KeyExchangeHandler";
}

void KeyExchangeHandler::processReceivedMessage(protocol::p2pMessage& message, MessageState& message_state) {
	protocol::p2pMessage_Payload payload = message.payload();
	if(payload.message_type() == payload.KEY_EXCHANGE){
		protocol::p2pMessage_Payload_KeyExchangePart deserialized_payload;
		deserialized_payload.ParseFromString(payload.serialized_payload());

		std::string src_pubkey = deserialized_payload.src_pubkey();
		if(crypto::Hash::compute(src_pubkey).toBinaryString() == message.header().src_th() &&
				crypto::PublicKeyDSA::fromBinaryString(src_pubkey).validate() &&
				crypto::PublicKeyDSA::fromBinaryString(src_pubkey).verify(src_pubkey, deserialized_payload.signature())){
			// So, public key is a valid ECDSA key, source TH matches Hash(Public Key) and public key is signed by its owner,
			// Now we know, that this message is genuine.
			// TODO: Generate ECDH key, if there is no such in NetDBStorage.
		}else{
			reject("Key validation failed.", message_state);
		}
	}
}

void KeyExchangeHandler::processSentMessage(protocol::p2pMessage& message, MessageState& message_state) {
}

} /* namespace handlers */
} /* namespace messaging */
} /* namespace p2pnet */
