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

#include "../protobuf/TransportSocketEndpoint_s.pb.h"
#include "TransportSocketEndpoint.h"
//#include "UDPTransportSocketEndpoint.h"
#include <memory>

namespace p2pnet {
namespace net {

void TransportSocketEndpoint::fromString(std::string endpoint_s){
	TransportSocketEndpoint_s tse_s;
	tse_s.ParseFromString(endpoint_s);
	fromProtobuf(tse_s);
}

std::string TransportSocketEndpoint::toString(){
	return toProtobuf().SerializeAsString();
}

} /* namespace net */
} /* namespace p2pnet */
