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
#include "OverlayPeer.h"
#include "../protobuf/Protocol.pb.h"

namespace p2pnet {
namespace overlay {

OverlaySocket::OverlaySocket() {}

OverlaySocket::~OverlaySocket() {}

std::shared_ptr<OverlayConnection> OverlaySocket::addConnection(overlay::TH th) {
	// TODO: Here we need to check peer.isActive() before.
	// Maybe, we will not be able to connect as that peer is lost (and we documented this)
	auto it_peer = m_peers.find(th);
	std::shared_ptr<OverlayPeer> peer;
	if(it_peer != m_peers.end()){
		peer = (*it_peer).second;
	}else{
		peer = std::make_shared<OverlayPeer>(th);
		m_peers.insert(std::make_pair(th, peer));
	}

	auto it_conn = m_connections.find(th);
	std::shared_ptr<OverlayConnection> connection;
	if(it_conn != m_connections.end()){
		connection = (*it_conn).second;
	}else{
		connection = std::make_shared<OverlayConnection>(peer);
		m_connections.insert(std::make_pair(th, connection));
	}
	return connection;
}

void OverlaySocket::send(const overlay::TH& dest,
		const protocol::OverlayMessage_Payload& message_payload,
		protocol::OverlayMessage_Header_MessagePriority prio) {
	addConnection(dest)->send(message_payload, prio);
}

void OverlaySocket::process(std::string data, const transport::TransportSocketEndpoint& from) {
	protocol::OverlayMessage overlay_message;
	protocol::ConnectionRequestMessage request_message;
	if(overlay_message.ParseFromString(data)){
		overlay::TH packet_src_th(overlay::TH::fromBinaryString(overlay_message.header().src_th()));

		log() << "Received Overlay Message from: TH:" << overlay::TH::fromBinaryString(overlay_message.header().src_th()).toBase58() << std::endl;

		addConnection(packet_src_th)->process(overlay_message, from);
	}else if(request_message.ParseFromString(data)){
		overlay::TH packet_src_th(overlay::TH::fromBinaryString(request_message.src_th()));

		log() << "Received Connection Request from: TH:" << overlay::TH::fromBinaryString(request_message.src_th()).toBase58() << std::endl;

		addConnection(packet_src_th)->process(request_message, from);
	}
}

} /* namespace overlay */
} /* namespace p2pnet */
