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

#include "TransportSocketMessageDispatcher.h"

namespace p2pnet {
namespace messaging {

TransportSocketMessageDispatcher::TransportSocketMessageDispatcher() {}
TransportSocketMessageDispatcher::~TransportSocketMessageDispatcher() {}

MessageHandler* TransportSocketMessageDispatcher::getHandlerByType(
		protocol::p2pMessage_MessageType message_type) {
	if(message_type == protocol::p2pMessage::AGREEMENT){
		return &cryptohandler;
	}
	return &nullhandler;
}

protocol::p2pMessage_MessageType TransportSocketMessageDispatcher::getMessageType(
		net::MessageBundle message_bundle) {
	protocol::p2pMessage message;
	message.ParseFromString(message_bundle.message);
	return message.message_type();
}

void TransportSocketMessageDispatcher::receivedMessage(
		net::MessageBundle message_bundle) {
	getHandlerByType(getMessageType(message_bundle))->receivedMessage(message_bundle);
}

void TransportSocketMessageDispatcher::sentMessage(
		net::MessageBundle message_bundle) {
	getHandlerByType(getMessageType(message_bundle))->sentMessage(message_bundle);
}

} /* namespace messaging */
} /* namespace p2pnet */
