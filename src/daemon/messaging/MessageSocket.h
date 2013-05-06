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

#include "../databases/PersonalKeyStorage.h"
#include "MessageGenerator.h"
#include "../net/TransportSocketListener.h"

namespace p2pnet {
namespace messaging {

class MessageSocket : public net::TransportSocketListener {
	databases::PersonalKeyStorage* m_pks;
	MessageGenerator m_generator;

	bool handleCrypto();
public:
	MessageSocket();
	virtual ~MessageSocket();

	// TransportSocketListener functions.
	virtual void receivedMessage(net::MessageBundle message_bundle);
	virtual void sentMessage(net::MessageBundle message_bundle);


};

} /* namespace messaging */
} /* namespace p2pnet */
#endif /* MESSAGESOCKET_H_ */
