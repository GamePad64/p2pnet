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
#include <set>
#include <memory>

namespace p2pnet {
namespace net {

class TransportSocketListener;
class TransportSocketEndpoint;

class TransportSocket {
protected:
	size_t max_packet_length;

	/**
	 * Creates MessageBundle for passing it to TransportSocketListeners.
	 * @param message Message (probably, in serialized Protocol Buffers format)
	 * @param endpoint
	 * @return
	 */
	MessageBundle createMessageBundle(const std::string message, TransportSocketEndpoint::pointer endpoint);
public:
	virtual ~TransportSocket();

	virtual void asyncReceiveFrom(TransportSocketEndpoint::pointer endpoint_p) = 0;
	void asyncReceiveFrom(const TransportSocketEndpoint& endpoint);
	virtual void asyncSendTo(TransportSocketEndpoint::pointer endpoint_p, const std::string& data) = 0;
	void asyncSendTo(const TransportSocketEndpoint& endpoint, const std::string& data);

	virtual void waitReceiveFrom(TransportSocketEndpoint::pointer endpoint_p) = 0;
	void waitReceiveFrom(const TransportSocketEndpoint& endpoint);
	virtual void waitSendTo(TransportSocketEndpoint::pointer endpoint_p, const std::string& data) = 0;
	void waitSendTo(const TransportSocketEndpoint& endpoint, const std::string& data);

	virtual MessageBundle hereReceiveFrom(TransportSocketEndpoint::pointer endpoint_p) = 0;
	MessageBundle hereReceiveFrom(TransportSocketEndpoint& endpoint);
	virtual void hereSendTo(TransportSocketEndpoint::pointer endpoint_p, const std::string& data) = 0;
	void hereSendTo(TransportSocketEndpoint& endpoint, const std::string& data);

	size_t getMaxPacketLength() const {return max_packet_length;};

	// Classic GoF Observer pattern.
private:
	/**
	 * Set of pointers to TransportSocketListener, like an array from GoF.
	 * These will be updated of a message is sent or received on this socket.
	 */
	std::set<TransportSocketListener*> m_listenerlist;
public:
	/**
	 * Adds listener to list to be updated on send/receive events.
	 * @param listener Pointer to TransportSocketListener.
	 */
	void addListener(TransportSocketListener* listener){m_listenerlist.insert(listener);};
	/**
	 * Prevents listener from updating by this socket.
	 * @param listener
	 */
	void removeListener(TransportSocketListener* listener){m_listenerlist.erase(listener);};
	void updateOnReceive(MessageBundle bundle);
	void updateOnSend(MessageBundle bundle);
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* MESSAGESOCKET_H_ */