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
#include "OverlaySocket.h"
#include "OverlayConnection.h"
#include "../protobuf/Protocol.pb.h"

namespace p2pnet {
namespace overlay {

OverlaySocket::OverlaySocket() : key_provider(this), dht_service(this) {}

OverlaySocket::~OverlaySocket() {}

void OverlaySocket::send(const TH& dest,
		const protocol::OverlayMessage_Payload& message_payload, Priority prio) {
	getNodeDB()->getNode(dest)->getConnection()->send(message_payload, prio);
}

void OverlaySocket::process(std::string data, const transport::TransportSocketEndpoint& from) {
	protocol::OverlayMessage overlay_message;
	protocol::ConnectionRequestMessage request_message;

	if(banned_peer_list.find(from) != banned_peer_list.end){
		log() << "<- Message from banned" << from.toReadableString() << std::endl;
		return;
	}

	if(overlay_message.ParseFromString(data)){
		overlay::TH packet_src_th(overlay::TH::fromBinaryString(overlay_message.header().src_th()));

		log() << "<- OverlayMessage: TH:" << overlay::TH::fromBinaryString(overlay_message.header().src_th()).toBase58() << std::endl;

		getNodeDB()->getNode(packet_src_th)->getConnection()->process(overlay_message, from);
	}else if(!getValue<bool>("policies.outgoing_only") && request_message.ParseFromString(data)){
		overlay::TH packet_src_th(overlay::TH::fromBinaryString(request_message.src_th()));

		log() << "<- Connection Request: TH:" << overlay::TH::fromBinaryString(request_message.src_th()).toBase58() << std::endl;

		getNodeDB()->getNode(packet_src_th)->updateEndpoint(from);
		if(!(getNodeDB()->getNode(packet_src_th)->hasConnection())){
			getNodeDB()->getNode(packet_src_th)->getConnection()->connect();
		}
	}
}

} /* namespace overlay */
} /* namespace p2pnet */
