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
#include <iostream>

namespace p2pnet {
namespace messaging {

CryptoHandler::CryptoHandler() {}
CryptoHandler::~CryptoHandler() {}

void CryptoHandler::processAgreement(protocol::p2pMessage parsed_message, net::MessageBundle message_bundle) {
	protocol::AgreementPart message_agreement = parsed_message.agreement();

	std::clog << "[CryptoHandler] " << "Received agreement request from: " << message_bundle.socket_link.getEndpointPtr()->toHRString() << " about: " << this->getSourceTH(parsed_message).toBase58() << std::endl;
}

void CryptoHandler::receivedMessage(net::MessageBundle message_bundle) {
	protocol::p2pMessage message;

	message.ParseFromString(message_bundle.message);
	if(message.message_type() == message.AGREEMENT){
		processAgreement(message, message_bundle);
	}


}

void CryptoHandler::sentMessage(net::MessageBundle message_bundle) {
}

} /* namespace messaging */
} /* namespace p2pnet */
