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

#ifndef MESSAGEHANDLER_H_
#define MESSAGEHANDLER_H_

#include "../../protobuf/Protocol.pb.h"
#include "../../databases/PersonalKeyStorage.h"
#include "../MessageGenerator.h"
#include <memory>

namespace p2pnet {
namespace messaging {

class MessageSocket;

namespace handlers {

class MessageHandler {
protected:
	MessageSocket* m_socket_ptr;
	databases::PersonalKeyStorage* m_pks;

	MessageGenerator m_generator;
public:
	MessageHandler(MessageSocket* socket_ptr){
		m_socket_ptr = socket_ptr;
		m_pks = databases::PersonalKeyStorage::getInstance();
	}
	virtual ~MessageHandler(){}

	struct MessageState {
		/**
		 * bool changed;
		 * Message contents were changed previously.
		 */
		bool changed;
		/**
		 * bool unencrypted;
		 * Serialized message payload was unencrypted (useless without "changed")
		 */
		bool unencrypted;
		/**
		 * bool repeat;
		 * This flag means, that this message will be processed again. Useful with "changed" and "unencrypted".
		 */
		bool repeat;
		/**
		 * bool skip;
		 * This flag means, that all next handlers will be skipped.
		 * If "repeat" is true, then processing starts from the beginning.
		 * Else, the message will be dropped.
		 */
		bool skip;
	};

	virtual void processReceivedMessage(protocol::p2pMessage& message, MessageState& message_props){}
	virtual void processSentMessage(protocol::p2pMessage& message, MessageState& message_props){}	// Well, not used usually.
};

} /* namespace handlers */
} /* namespace messaging */
} /* namespace p2pnet */

#endif /* MESSAGEHANDLER_H_ */
