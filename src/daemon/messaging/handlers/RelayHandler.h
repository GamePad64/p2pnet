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

#ifndef RELAYHANDLER_H_
#define RELAYHANDLER_H_

#include "MessageHandler.h"

namespace p2pnet {
namespace messaging {
namespace handlers {

class RelayHandler : MessageHandler {
public:
	RelayHandler(MessageSocket* socket_ptr);
	virtual ~RelayHandler();

	void processReceivedMessage(protocol::p2pMessage& message, MessageState& message_props);
	void processSentMessage(protocol::p2pMessage& message, MessageState& message_props);
};

} /* namespace handlers */
} /* namespace messaging */
} /* namespace p2pnet */
#endif /* RELAYHANDLER_H_ */
