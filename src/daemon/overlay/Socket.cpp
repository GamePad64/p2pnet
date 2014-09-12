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
#include "Socket.h"
#include "Connection.h"
#include "Protocol.pb.h"

namespace p2pnet {
namespace overlay {

Socket::Socket() : key_provider(this), dht_service(this) {}

Socket::~Socket() {}

void Socket::send(const TH& dest,
		const protocol::OverlayMessage_Payload& message_payload, Priority prio) {
	getNodeDB()->getConnection(dest)->send(message_payload, prio);
}

void Socket::process(std::string data, const transport::SocketEndpoint& from) {
	protocol::OverlayMessage overlay_message;
	protocol::ConnectionRequestMessage request_message;

	if(overlay_message.ParseFromString(data)){
		overlay::TH packet_src_th(overlay::TH::fromBinaryString(overlay_message.header().src_th()));

		log() << "<- OverlayMessage: TH:" << overlay::TH::fromBinaryString(overlay_message.header().src_th()).toBase58() << std::endl;

		getNodeDB()->getConnection(packet_src_th)->process(overlay_message, from);
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
