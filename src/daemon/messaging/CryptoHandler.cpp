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
#include <iostream>

namespace p2pnet {
namespace messaging {

CryptoHandler::CryptoHandler() {}
CryptoHandler::~CryptoHandler() {}

void CryptoHandler::receivedMessage(net::MessageBundle message_bundle){
	protocol::p2pMessage message;
	protocol::p2pMessage_Agreement message_agreement;

	message.ParseFromString(message_bundle.message);
	message_agreement.ParseFromString(message.message_s());

	crypto::hash_t sender_id(message.release_message_header()->src_id().begin(), message.release_message_header()->src_id().end());
	std::clog << "[CryptoHandler] " << "Received agreement request from: " << crypto::hashToHex(sender_id);
};
void CryptoHandler::sentMessage(net::MessageBundle message_bundle){};

} /* namespace messaging */
} /* namespace p2pnet */
