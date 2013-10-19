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

#ifndef DISCOVERYSERVICE_H_
#define DISCOVERYSERVICE_H_

#include "../../common/Config.h"
#include "../../common/Loggable.h"
#include "../transport/TransportSocketEndpoint.h"
#include "../protobuf/Protocol.pb.h"

namespace p2pnet {
namespace discovery {

class DiscoveryService : public ConfigClient, public Loggable {
public:
	DiscoveryService();
	virtual ~DiscoveryService();

	protocol::OverlayMessageStructure generateHandshakeMessage();

	void handshake(transport::TransportSocketEndpoint endpoint);
};

} /* namespace discovery */
} /* namespace p2pnet */
#endif /* DISCOVERYSERVICE_H_ */
