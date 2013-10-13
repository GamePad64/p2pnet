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
#include "../../common/Singleton.h"

namespace p2pnet {
namespace transport {

class TransportSocketListener;
class TransportSocketEndpoint;
class TransportConnection;

class TransportSocket : public Singleton<TransportSocket>, public Loggable {
protected:
	std::map<uint32_t, std::shared_ptr<TransportInterface>> interfaces;
	std::map<std::string, std::shared_ptr<TransportInterface>> readable_strings_prefixes;

	/**
	 * Set of pointers to TransportSocketListener, like an array from GoF.
	 * These will be updated of a message is sent or received on this socket.
	 */
	std::list<TransportSocketListener*> m_listenerlist;
public:
	TransportSocket();
	virtual ~TransportSocket();

	// Made it public, because interfaces, connections and other stuff have to lookup this table.
	std::map<TransportSocketEndpoint, std::shared_ptr<TransportConnection>> m_connections;

	std::shared_ptr<TransportInterface> getInterfaceByID(uint32_t id);
	std::shared_ptr<TransportInterface> getInterfaceByPrefix(std::string prefix);
	void registerInterface(std::shared_ptr<TransportInterface> interface);

//	This was used in the past, now uplevel-interconnection is handled by TransportConnection.
//	// All data I/O is processed using classic GoF Observer pattern.
//	/**
//	 * Adds listener to list to be updated on send/receive events.
//	 * @param listener Pointer to TransportSocketListener.
//	 */
//	void addListener(TransportSocketListener* listener){m_listenerlist.push_back(listener);};
//	/**
//	 * Prevents listener from updating by this socket.
//	 * @param listener
//	 */
//	void removeListener(TransportSocketListener* listener){m_listenerlist.remove(listener);};
//	void updateOnReceive(TransportSocketCallback callback);

	std::string getComponentName(){return "TransportSocket";}

	// Basically, I/O
	void receive();
	void send(TransportSocketEndpoint endpoint, const std::string& data);
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* TRANSPORTSOCKET_H_ */
