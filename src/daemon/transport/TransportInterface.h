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

#include "../../common/Config.h"
#include "TransportSocketEndpoint.h"
#include <memory>
#include <cstdint>

namespace p2pnet {
namespace transport {

class TransportInterfaceEndpoint;
class TransportSocketEndpoint;

class TransportInterface : public ConfigClient {
public:
	TransportInterface() {};
	virtual ~TransportInterface(){};

	/*
	 * TransportSocket registration information
	 */
	virtual uint32_t getInterfaceID() const = 0;
	virtual std::string getInterfacePrefix() const = 0;
	inline bool checkEndpoint(TransportSocketEndpoint endpoint) const {
		return (getInterfaceID() == endpoint.getInterfaceID());
	}

	virtual std::shared_ptr<TransportInterfaceEndpoint> createEndpoint() = 0;

	/*
	 * Interface network I/O functions.
	 */
	virtual void receive() = 0;
	virtual void send(TransportSocketEndpoint, const std::string& data) = 0;
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* TRANSPORTINTERFACE_H_ */
