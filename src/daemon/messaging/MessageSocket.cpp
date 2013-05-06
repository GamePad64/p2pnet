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

#include "MessageSocket.h"

namespace p2pnet {
namespace messaging {

MessageSocket::MessageSocket() {
	m_pks = databases::PersonalKeyStorage::getInstance();
}

MessageSocket::~MessageSocket() {
	// TODO Auto-generated destructor stub
}

void MessageSocket::receivedMessage(net::MessageBundle message_bundle) {
	protocol::p2pMessage message_orig, message_new;
	message_orig.ParseFromString(message_bundle.message);

	message_new = message_orig;

	if(! m_generator.checkMessageCRC32(message_orig)){
		std::clog << "[MessageSocket] Rejected message: CRC-32 mismatch." << std::endl;
		return;	// Drop!
	}

	if(message_orig.payload().message_type() == protocol::p2pMessage_Payload_MessageType_KEY_EXCHANGE ||
			message_orig.payload().message_type() == protocol::p2pMessage_Payload_MessageType_AGREEMENT){

	}

}

void MessageSocket::sentMessage(net::MessageBundle message_bundle) {
}

} /* namespace messaging */
} /* namespace p2pnet */
