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

#include "AgreementHandler.h"

namespace p2pnet {
namespace messaging {
namespace handlers {

AgreementHandler::AgreementHandler(MessageSocket* socket_ptr) : MessageHandler(socket_ptr) {}
AgreementHandler::~AgreementHandler() {}

std::string AgreementHandler::getHandlerName() {
	return "AgreementHandler";
}

void AgreementHandler::processReceivedMessage(protocol::p2pMessage& message, MessageState& message_state, Session::pointer session_ptr) {
	protocol::p2pMessage_Payload payload = message.payload();
	if(payload.message_type() == payload.AGREEMENT){
		protocol::p2pMessage_Payload_AgreementPart deserialized_payload;
		deserialized_payload.ParseFromString(payload.serialized_payload());

		peer::TH src_th = peer::TH::fromBinaryString(message.header().src_th());

		// So, we need to get this from NetDB.
//		if(databases::NetDBStorage::getInstance()->hasEntry(src_th)){
		std::string src_pubkey_s = databases::NetDBStorage::getInstance()->getEntry(src_th).ecdsa_public_key();
//		} else {
//			EXCEPTION: We need to check if we have this peer's key in NetDB. If not,
//			           this message must be processed in other way.
//		}
		crypto::PublicKeyDSA src_pubkey = crypto::PublicKeyDSA::fromBinaryString(src_pubkey_s);

		if(!src_pubkey.verify(deserialized_payload.src_ecdh_pubkey(), deserialized_payload.signature()))
			reject(Reason::KEY_INVALID, "Signature is invalid");

		// So, we have received ECDH public key, signed by sender.
		// This key is genuine (signature is valid).
		// So, we need to generate new ECDH key, if there is no such in Session.
		if(session_ptr->hasECDHPrivateKey()){	// It means, that we sent the key earlier!

		}
	}
}

void AgreementHandler::processSentMessage(protocol::p2pMessage& message, MessageState& message_state, Session::pointer session_ptr) {
}

} /* namespace handlers */
} /* namespace messaging */
} /* namespace p2pnet */
