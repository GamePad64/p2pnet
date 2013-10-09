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
#include "OverlayConnection.h"
#include <algorithm>

namespace p2pnet {
namespace overlay {

OverlayConnection::OverlayConnection(overlay::TH th) : th_endpoint(th) {}
OverlayConnection::~OverlayConnection() {}

bool OverlayConnection::isReady() const {
	return !(m_tse.empty());
}

void OverlayConnection::send(std::string data) {

}

void OverlayConnection::process(std::string data, transport::TransportSocketEndpoint from) {
	protocol::OverlayMessageStructure message;
	message.ParseFromString(data);

	auto it = std::find(m_tse.begin(), m_tse.end(), from);
	if(it == m_tse.end()){
		m_tse.push_front(from);
	}

	auto pks = databases::PersonalKeyStorage::getInstance();
	overlay::TH dest_th = overlay::TH::fromBinaryString(message.header().dest_th());

	if(pks->getPrivateKeyOfTH(dest_th) == nullptr){
		// This message is completely stale, or it is intended to be retransmitted.
	}else{
		auto payload = message.payload();

		switch(payload.message_type()){
		case payload.KEY_REQUEST:

			break;
		}
	}
}

} /* namespace overlay */
} /* namespace p2pnet */
