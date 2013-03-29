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

#include "../protobuf/Protocol.pb.h"
#include "../net/TransportSocketListener.h"
#include "../../common/crypto/CryptoTypes.h"

namespace p2pnet {
namespace messaging {

class MessageHandler {
protected:
	crypto::hash_t getSourceTH(const protocol::p2pMessage& message);
public:
	MessageHandler(){};
	virtual ~MessageHandler(){};

	virtual void receivedMessage(net::MessageBundle message_bundle){};
	virtual void sentMessage(net::MessageBundle message_bundle){};	// Well, not used usually.
};

} /* namespace messaging */
} /* namespace p2pnet */

#endif /* MESSAGEHANDLER_H_ */
