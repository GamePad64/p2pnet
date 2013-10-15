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
#ifndef TRANSPORTCONNECTION_H_
#define TRANSPORTCONNECTION_H_

#include "TransportSocketEndpoint.h"
#include "../overlay/OverlaySocket.h"
#include "../../common/Loggable.h"

namespace p2pnet {
namespace transport {

class TransportConnection : public std::enable_shared_from_this<TransportConnection>, public Loggable {
protected:
	TransportSocketEndpoint m_endpoint;
public:
	TransportConnection(TransportSocketEndpoint endpoint);
	virtual ~TransportConnection();

	virtual void send(std::string data) = 0;
	/**
	 * Used for transferring data from TransportConnection to OverlayConnection.
	 * This method is something like an elevator, transferring data from low-level class to high-level.
	 * May be overrided to provide some additional functionality. For ex. UDP ping.
	 * @param data
	 */
	virtual void process(std::string data);

	virtual bool connected() = 0;

	std::string getComponentName(){return "TransportConnection";}
};

} /* namespace net */
} /* namespace p2pnet */

#endif /* TRANSPORTCONNECTION_H_ */
