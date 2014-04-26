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
#ifndef GENERICDESTINATION_H_
#define GENERICDESTINATION_H_

#include <map>
#include <memory>

namespace p2pnet {
namespace endpoint {

class EndpointSocket;

class GenericDestination {
protected:
	// Maps connected_socket_id to pointer
	std::map<uint32_t, std::shared_ptr<EndpointSocket>> dest_connections;
public:
	GenericDestination();
	virtual ~GenericDestination();

	virtual void handleConnectRequest(std::shared_ptr<GenericDestination> endpoint) = 0;
};

} /* namespace endpoint */
} /* namespace p2pnet */

#endif /* GENERICDESTINATION_H_ */
