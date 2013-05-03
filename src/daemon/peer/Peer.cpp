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

#include "Peer.h"
#include <iostream>

namespace p2pnet {
namespace peer {

Peer::Peer(peer::TH th) : m_th(th) {}
Peer::~Peer() {}

bool Peer::hasRoute(const net::TransportSocketEndpoint& route) {
	/*
	 * Yes, it is a fucking scary function.
	 * It uses protobuf serialization for comparison =(
	 * It will be fixed, when we move the database handling to SQLite.
	 */
	auto route_s = route.toProtobuf();
	std::string route_str = route_s.SerializeAsString();

	auto i = getAllRoutes().begin();

	while(i != getAllRoutes().end()){
		if(route_str == *i)
			return true;
		i++;
	}
	return false;
}

void Peer::addRoute(const net::TransportSocketEndpoint& route) {
	getAllRoutes().push_front(route.toProtobuf().SerializeAsString());
}

} /* namespace peer */
} /* namespace p2pnet */
