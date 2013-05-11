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

#include "MessageSocket.h"
#include <iostream>

namespace p2pnet {
namespace messaging {

MessageSocket::MessageSocket() {
	m_pks = databases::PersonalKeyStorage::getInstance();
}

MessageSocket::~MessageSocket() {}

void MessageSocket::receivedMessage(net::MessageBundle message_bundle) {
	protocol::p2pMessage message;
	message.ParseFromString(message_bundle.message);

	// TODO: Add this TransportSocketLink to database.

	processReceivedMessage(message);
}

void MessageSocket::sentMessage(net::MessageBundle message_bundle) {
}

void MessageSocket::processReceivedMessage(protocol::p2pMessage message) {
	handlers::MessageHandler::MessageState message_props;

	do {
		message_props.repeat = false;
		for(auto handler : m_handler_list){
			handler->processReceivedMessage(message, message_props);
			if(message_props.skip){
				break;
			}
		}
	} while(message_props.repeat);
}

void MessageSocket::addHandler(handlers::MessageHandler* handler_ptr) {
	m_handler_list.push_back(handler_ptr);
}

void MessageSocket::removeHandler(handlers::MessageHandler* handler_ptr) {
	m_handler_list.remove(handler_ptr);
}

} /* namespace messaging */
} /* namespace p2pnet */
