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

OverlaySocket::OverlaySocket() {}

OverlaySocket::~OverlaySocket() {}

std::shared_ptr<OverlayConnection> OverlaySocket::addConnection(overlay::TH th) {
	auto it = m_connections.find(th);
	std::shared_ptr<OverlayConnection> connection;
	if(it != m_connections.end()){
		connection = (*it).second;
	}else{
		connection = std::make_shared<OverlayConnection>(th);
		m_connections.insert(std::make_pair(th, connection));
	}
	return connection;
}

void OverlaySocket::send(overlay::TH dest, std::string data) {
	auto connection = addConnection(dest);
	// TODO: some sort of DHT.
}

void OverlaySocket::process(std::string data, transport::TransportSocketEndpoint from) {
	protocol::OverlayMessage overlay_message;
	protocol::ConnectionRequestMessage request_message;
	if(overlay_message.ParseFromString(data)){
		overlay::TH packet_src_th(overlay::TH::fromBinaryString(overlay_message.header().src_th()));

		addConnection(packet_src_th)->process(overlay_message, from);
	}else if(request_message.ParseFromString(data)){
		overlay::TH packet_src_th(overlay::TH::fromBinaryString(request_message.src_th()));

		addConnection(packet_src_th)->process(request_message, from);
	}
}

} /* namespace overlay */
} /* namespace p2pnet */
