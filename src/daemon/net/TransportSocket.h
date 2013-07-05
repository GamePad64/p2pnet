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

#ifndef TRANSPORTSOCKET_H_
#define TRANSPORTSOCKET_H_

#include "TransportInterface.h"
#include <string>
#include <list>
#include <memory>
#include <vector>
#include <map>
#include "../abstract/Singleton.h"

namespace p2pnet {
namespace net {

class TransportSocketListener;
class TransportSocketEndpoint;

class TransportSocket : public abstract::Singleton<TransportSocket> {
protected:
	std::vector<TransportInterface*> interfaces;
	std::map<std::string, TransportInterface*> readable_strings_prefixes;

	/**
	 * Set of pointers to TransportSocketListener, like an array from GoF.
	 * These will be updated of a message is sent or received on this socket.
	 */
	std::list<TransportSocketListener*> m_listenerlist;
public:
	virtual ~TransportSocket();

	typedef std::tuple<TransportInterfaceEndpoint::pointer, std::string> Callback;

	TransportInterface* getInterfaceByID(uint32_t id);
	TransportInterface* getInterfaceByPrefix(std::string prefix);
	void registerInterface(TransportInterface* interface);

	// All data I/O is processed using classic GoF Observer pattern.
	/**
	 * Adds listener to list to be updated on send/receive events.
	 * @param listener Pointer to TransportSocketListener.
	 */
	void addListener(TransportSocketListener* listener){m_listenerlist.push_back(listener);};
	/**
	 * Prevents listener from updating by this socket.
	 * @param listener
	 */
	void removeListener(TransportSocketListener* listener){m_listenerlist.remove(listener);};
	void updateOnReceive(TransportSocketCallback callback);
	void updateOnSend(TransportSocketCallback callback);

	// Basically, I/O
	void asyncReceiveFrom(TransportSocketEndpoint endpoint);
	void waitReceiveFrom(TransportSocketEndpoint endpoint);
	TransportSocketCallback hereReceiveFrom(TransportSocketEndpoint endpoint);

	void asyncSendTo(TransportSocketEndpoint endpoint, const std::string& data);
	void waitSendTo(TransportSocketEndpoint endpoint, const std::string& data);
	void hereSendTo(TransportSocketEndpoint endpoint, const std::string& data);
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* TRANSPORTSOCKET_H_ */
