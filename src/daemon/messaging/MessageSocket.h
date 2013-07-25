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

#ifndef MESSAGESOCKET_H_
#define MESSAGESOCKET_H_

#include "handlers/MessageHandler.h"

#include "RejectException.h"
#include "MessageGenerator.h"
#include "SessionMap.h"

#include "../databases/PersonalKeyStorage.h"
#include "../net/TransportSocketListener.h"
#include <list>

namespace p2pnet {
namespace messaging {

class MessageSocket : public net::TransportSocketListener {
	// Active handlers
	std::list<handlers::MessageHandler*> m_handler_list;

	// Other stuff
	databases::PersonalKeyStorage* m_pks;

	void reject(Reason reason);
	void processReceivedMessage(protocol::p2pMessage message);
public:
	MessageSocket();
	virtual ~MessageSocket();

	void sendMessage(peer::TH th, protocol::p2pMessage message);

	// TransportSocketListener functions.
	virtual void receivedMessage(net::TransportSocketCallback callback);
	virtual void sentMessage(net::TransportSocketCallback callback);

	void addHandler(handlers::MessageHandler* handler_ptr);
	void removeHandler(handlers::MessageHandler* handler_ptr);
};

} /* namespace messaging */
} /* namespace p2pnet */
#endif /* MESSAGESOCKET_H_ */
