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

#ifndef CONNECTIONHANDLER_H_
#define CONNECTIONHANDLER_H_

#include "MessageHandler.h"
#include "../../databases/NetDBStorage.h"

namespace p2pnet {
namespace messaging {
namespace handlers {

class ConnectionHandler : public p2pnet::messaging::handlers::MessageHandler {
	databases::NetDBStorage* m_netdb_storage;
	std::string getHandlerName();
public:
	ConnectionHandler(MessageSocket* socket_ptr);
	virtual ~ConnectionHandler();

	void processReceivedMessage(protocol::p2pMessage& message, MessageState& message_state, Session::pointer session_ptr);
	void processSentMessage(protocol::p2pMessage& message, MessageState& message_state, Session::pointer session_ptr);
};

} /* namespace handlers */
} /* namespace messaging */
} /* namespace p2pnet */
#endif /* CONNECTIONHANDLER_H_ */
