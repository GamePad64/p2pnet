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
#ifndef ENDPOINTMANAGER_H_
#define ENDPOINTMANAGER_H_

#include "../../common/Singleton.h"
#include "../../common/Loggable.h"
#include "../../common/Config.h"
#include "SH.h"

#include <memory>
#include <map>

namespace p2pnet {
namespace endpoint {

class EndpointSocket;
class GenericDestination;

class EndpointManager : public Singleton<EndpointManager>, Loggable, ConfigClient {
	std::map<SH, std::shared_ptr<EndpointSocket>> bound_local_sockets;
	std::set<std::shared_ptr<EndpointSocket>> unbound_local_sockets;

	std::map<SH, std::shared_ptr<GenericDestination>> remote_destinations;
public:
	EndpointManager();
	virtual ~EndpointManager();

	std::shared_ptr<GenericDestination> getLoopbackDestination(SH sh);

	void setBound(std::shared_ptr<EndpointSocket> endpoint_socket_ptr);
	void setUnbound(std::shared_ptr<EndpointSocket> endpoint_socket_ptr);

	void registerSocket(std::shared_ptr<EndpointSocket> endpoint_socket_ptr);
	void unregisterSocket(std::shared_ptr<EndpointSocket> endpoint_socket_ptr);
};

} /* namespace endpoint */
} /* namespace p2pnet */

#endif /* ENDPOINTMANAGER_H_ */
