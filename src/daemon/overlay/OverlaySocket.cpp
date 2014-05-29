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

OverlaySocket::OverlaySocket() : m_connections(), key_provider(this), dht_service(this) {}

OverlaySocket::~OverlaySocket() {}

OverlayConnection* OverlaySocket::getConnection(const overlay::TH& th) {
	auto it_peer = m_connections.find(th);

	if(it_peer != m_connections.end()){
		return it_peer->second;
	}

	auto conn_ptr = new OverlayConnection(th);
	m_connections.insert(std::make_pair(th, conn_ptr));

	return conn_ptr;
}

void OverlaySocket::send(const TH& dest,
		const protocol::OverlayMessage_Payload& message_payload, Priority prio) {
	getConnection(dest)->send(message_payload, prio);
}

void OverlaySocket::process(std::string data, const transport::TransportSocketEndpoint& from) {
	protocol::OverlayMessage overlay_message;
	protocol::ConnectionRequestMessage request_message;
	if(overlay_message.ParseFromString(data)){
		overlay::TH packet_src_th(overlay::TH::fromBinaryString(overlay_message.header().src_th()));

		log() << "<- OverlayMessage: TH:" << overlay::TH::fromBinaryString(overlay_message.header().src_th()).toBase58() << std::endl;

		getConnection(packet_src_th)->process(overlay_message, from);
	}else if(request_message.ParseFromString(data)){
		overlay::TH packet_src_th(overlay::TH::fromBinaryString(request_message.src_th()));

		log() << "<- Connection Request: TH:" << overlay::TH::fromBinaryString(request_message.src_th()).toBase58() << std::endl;

		getConnection(packet_src_th)->process(request_message, from);
	}	// else drop
}

void OverlaySocket::removePeer(const overlay::TH& th) {
	m_connections.erase(th);
}

void OverlaySocket::movePeer(const overlay::TH& from, const overlay::TH& to) {
	m_connections[to] = m_connections[from];
}

void OverlaySocket::notifyKeysUpdated(std::pair<crypto::PrivateKeyDSA, TH> previous_keys, std::pair<crypto::PrivateKeyDSA, TH> new_keys){
	for(auto connection : m_connections){
		connection.second->performRemoteKeyRotation(previous_keys);
	}

	dht_service.recomputeAll();
}

OverlayKeyProvider* OverlaySocket::getKeyProvider() {
	return &key_provider;
}

OverlayDHT* OverlaySocket::getDHT() {
	return &dht_service;
}

} /* namespace overlay */
} /* namespace p2pnet */
