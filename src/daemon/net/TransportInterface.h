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

#ifndef TRANSPORTINTERFACE_H_
#define TRANSPORTINTERFACE_H_

#include "../protobuf/NetDBEntry.pb.h"
#include "TransportSocketListener.h"
#include <memory>
#include <stdint.h>

namespace p2pnet {
namespace net {

class TransportSocketEndpoint;
class TransportSocketCallback;

class TransportInterface {
public:
	TransportInterface(){};
	virtual ~TransportInterface(){};

	virtual TransportInterfaceEndpoint::pointer createEndpoint() = 0;

	/*
	 * TransportSocket registration information
	 */
	virtual uint32_t getInterfaceID() const {return 0;}
	virtual std::string getInterfacePrefix() const = 0;

	inline bool checkEndpoint(const TransportInterfaceEndpoint::const_pointer endpoint) const {
		return (getInterfaceID() == endpoint->getInterfaceID());
	}

	/*
	 * Interface network I/O functions.
	 */
	virtual void asyncReceiveFrom(TransportInterfaceEndpoint::const_pointer endpoint_p) = 0;
	virtual void waitReceiveFrom(TransportInterfaceEndpoint::const_pointer endpoint_p) = 0;
	virtual TransportSocketCallback hereReceiveFrom(TransportInterfaceEndpoint::const_pointer endpoint_p) = 0;

	virtual void asyncSendTo(TransportInterfaceEndpoint::const_pointer endpoint_p, const std::string& data) = 0;
	virtual void waitSendTo(TransportInterfaceEndpoint::const_pointer endpoint_p, const std::string& data) = 0;
	virtual void hereSendTo(TransportInterfaceEndpoint::const_pointer endpoint_p, const std::string& data) = 0;

	/*
	virtual void asyncAccept() = 0;
	virtual void waitAccept() = 0;
	virtual Stream hereAccept() = 0;
	TODO: Stream concept is not ready enough
	*/
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* TRANSPORTINTERFACE_H_ */
