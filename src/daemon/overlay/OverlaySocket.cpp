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

OverlaySocket::OverlaySocket() : dht_service(this) {}

OverlaySocket::~OverlaySocket() {}



std::shared_ptr<OverlayConnection> OverlaySocket::getConnection(const overlay::TH& th) {
	auto it_peer = m_peers_conns.find(th);

	if(it_peer != m_peers_conns.end()){
		return it_peer->second.connection;
	}

	auto new_peer = std::make_shared<OverlayPeer>(th);
	auto new_connection = std::make_shared<OverlayConnection>(new_peer);
	overlay_peer_conn_t peer_conn = {new_peer, new_connection};

	m_peers_conns.insert(std::make_pair(th, peer_conn));
	return peer_conn.connection;
}

std::shared_ptr<OverlayPeer> OverlaySocket::getPeer(const overlay::TH& th) {
	auto it_peer = m_peers_conns.find(th);

	if(it_peer != m_peers_conns.end()){
		return it_peer->second.peer;
	}

	auto new_peer = std::make_shared<OverlayPeer>(th);
	auto new_connection = std::make_shared<OverlayConnection>(new_peer);
	overlay_peer_conn_t peer_conn = {new_peer, new_connection};

	m_peers_conns.insert(std::make_pair(th, peer_conn));
	return peer_conn.peer;
}

void OverlaySocket::send(const overlay::TH& dest,
		const protocol::OverlayMessage_Payload& message_payload,
		protocol::OverlayMessage_Header_MessagePriority prio) {
	getConnection(dest)->send(message_payload, prio);
}

void OverlaySocket::process(std::string data, const transport::TransportSocketEndpoint& from) {
	protocol::OverlayMessage overlay_message;
	protocol::ConnectionRequestMessage request_message;
	if(overlay_message.ParseFromString(data)){
		overlay::TH packet_src_th(overlay::TH::fromBinaryString(overlay_message.header().src_th()));

		log() << "Received Overlay Message from: TH:" << overlay::TH::fromBinaryString(overlay_message.header().src_th()).toBase58() << std::endl;

		getConnection(packet_src_th)->process(overlay_message, from);
	}else if(request_message.ParseFromString(data)){
		overlay::TH packet_src_th(overlay::TH::fromBinaryString(request_message.src_th()));

		log() << "Received Connection Request from: TH:" << overlay::TH::fromBinaryString(request_message.src_th()).toBase58() << std::endl;

		getConnection(packet_src_th)->process(request_message, from);
	}	// else drop
}

void OverlaySocket::removePeer(const overlay::TH& th) {
	m_peers_conns.erase(th);
}

void OverlaySocket::movePeer(const overlay::TH& from, const overlay::TH& to) {
	m_peers_conns[to] = m_peers_conns[from];
}

} /* namespace overlay */
} /* namespace p2pnet */
