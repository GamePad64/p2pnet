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

#include "TransportSocketConnection.h"

#include <string>

namespace p2pnet {
namespace net {

class TransportSocketListener;
class TransportSocketEndpoint;

class TransportSocket {
protected:
	size_t max_packet_length;

	MessageBundle createMessageBundle(std::string message, TransportSocketEndpoint &endpoint, TransportSocketConnection connection);
public:
	virtual ~TransportSocket();

	virtual void asyncReceiveFrom(TransportSocketEndpoint &endpoint, TransportSocketListener* listener, TransportSocketConnection connection = TransportSocketConnection(0, 0)) = 0;
	virtual void asyncSendTo(TransportSocketEndpoint &endpoint, const std::string data, TransportSocketListener* listener, TransportSocketConnection connection = TransportSocketConnection(0, 0)) = 0;

	virtual void waitReceiveFrom(TransportSocketEndpoint &endpoint, TransportSocketListener* listener, TransportSocketConnection connection = TransportSocketConnection(0, 0)) = 0;
	virtual void waitSendTo(TransportSocketEndpoint &endpoint, const std::string data, TransportSocketListener* listener, TransportSocketConnection connection = TransportSocketConnection(0, 0)) = 0;

	virtual MessageBundle hereReceiveFrom(TransportSocketEndpoint &endpoint, TransportSocketConnection connection = TransportSocketConnection(0, 0)) = 0;
	virtual void hereSendTo(TransportSocketEndpoint &endpoint, const std::string data) = 0;

	size_t getMaxPacketLength() const {return max_packet_length;};
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* MESSAGESOCKET_H_ */
