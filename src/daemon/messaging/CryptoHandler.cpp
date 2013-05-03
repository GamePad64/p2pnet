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

#include "CryptoHandler.h"
#include "../../common/crypto/Hash.h"
#include "../peer/Peer.h"
#include <iostream>

namespace p2pnet {
namespace messaging {

CryptoHandler::CryptoHandler(databases::NetDBStorage& netdb_storage) : m_netdb_storage(netdb_storage) {}
CryptoHandler::~CryptoHandler() {
}

void CryptoHandler::processAgreement(protocol::p2pMessage parsed_message,
		net::MessageBundle message_bundle) {

	crypto::Hash src_hash = this->getSourceTH(parsed_message);

	std::clog << "[CryptoHandler] " << "Received Public Key from: "
			<< message_bundle.socket_link.getEndpointPtr()->toHRString()
			<< " about: " << src_hash.toBase58()
			<< std::endl;

}

void CryptoHandler::receivedMessage(net::MessageBundle message_bundle) {
	protocol::p2pMessage message;

	message.ParseFromString(message_bundle.message);
	if (message.payload().message_type() == protocol::p2pMessage::Payload::MessageType::p2pMessage_Payload_MessageType_KEY_EXCHANGE) {
		processAgreement(message, message_bundle);
	}

}

void CryptoHandler::sentMessage(net::MessageBundle message_bundle) {
}

} /* namespace messaging */
} /* namespace p2pnet */
