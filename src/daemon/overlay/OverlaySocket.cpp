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
#include "../protobuf/Protocol.pb.h"

namespace p2pnet {
namespace overlay {

OverlaySocket::OverlaySocket() {}

OverlaySocket::~OverlaySocket() {}

void OverlaySocket::send(overlay::TH dest, std::string data) {
	auto it = m_connections.find(dest);
	std::shared_ptr<OverlayConnection> connection;

	if(it != m_connections.end()){
		connection = *it;
	}else{
		connection = std::make_shared<OverlayConnection>(dest);
	}
	// TODO: some sort of DHT.
}

void OverlaySocket::process(std::string data, transport::TransportSocketEndpoint from) {
	protocol::OverlayMessageStructure overlay_message;
	if(overlay_message.ParseFromString(data)){
		auto header = overlay_message.header();
		overlay::TH packet_src_th(overlay::TH::fromBinaryString(header.src_th()));

		auto it = m_connections.find(packet_src_th);
		std::shared_ptr<OverlayConnection> connection;
		if(it != m_connections.end()){
			connection = *it;
		}else{
			connection = std::make_shared<OverlayConnection>(packet_src_th);
		}

		connection->process(data, from);
	}
}

} /* namespace overlay */
} /* namespace p2pnet */
