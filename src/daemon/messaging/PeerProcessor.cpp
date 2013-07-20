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

#include "PeerProcessor.h"
#include "../databases/NetDBStorage.h"
#include "../messaging/SessionMap.h"
#include "../protobuf/NetDBEntry.pb.h"

namespace p2pnet {
namespace messaging {

PeerProcessor::PeerProcessor() {}
PeerProcessor::~PeerProcessor() {}

void PeerProcessor::processNewPeerConnection(peer::TH th, net::TransportSocketEndpoint endpoint) {
	if(! databases::NetDBStorage::getInstance()->hasEntry(th)){
		std::clog << "[" << getComponentName() << "] Discovered peer: " << th.toBase58() << std::endl;
	}

	databases::NetDBEntry& db_entry = databases::NetDBStorage::getInstance()->getEntry(th);

	if(! databases::NetDBStorage::getInstance()->hasRouteToPeer(th, endpoint.toProtobuf()) ){
		databases::TransportSocketEndpoint_s* db_endpoint = db_entry.mutable_endpoints()->Add();
		*db_endpoint = endpoint.toProtobuf();
		db_endpoint->set_last_usage((google::protobuf::int64)std::time(0));
	}else{
		/*
		 * This function should be called when we want to "bump" selected route,
		 * so it is set to preferred.
		 */
		databases::NetDBStorage::getInstance()->bumpRouteToPeer(th, endpoint.toProtobuf());
	}

	auto session_ptr = (*messaging::SessionStorage::getInstance())[th.toBinaryString()];
	session_ptr->sendConnectionMessage();
}

} /* namespace messaging */
} /* namespace p2pnet */
