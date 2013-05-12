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

#include "RelayHandler.h"
#include <iostream>

namespace p2pnet {
namespace messaging {
namespace handlers {

RelayHandler::RelayHandler(MessageSocket* socket_ptr) : MessageHandler(socket_ptr) {}
RelayHandler::~RelayHandler() {}

void RelayHandler::processReceivedMessage(protocol::p2pMessage& message, MessageState& message_props) {
	if (message.header().has_dest_th()
			&& message.header().dest_th() != m_pks->getMyTransportHash().toBinaryString()){
		// TODO Relay.
		std::clog << "[RelayHandler] Relay message received. Not implemented yet." << std::endl;
		message_props.skip = true;
	}
}

void RelayHandler::processSentMessage(protocol::p2pMessage& message, MessageState& message_props) {

}

} /* namespace handlers */
} /* namespace messaging */
} /* namespace p2pnet */
