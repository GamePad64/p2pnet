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

#include "CRC32Handler.h"

namespace p2pnet {
namespace messaging {
namespace handlers {

CRC32Handler::CRC32Handler(MessageSocket* socket_ptr) : MessageHandler(socket_ptr) {}
CRC32Handler::~CRC32Handler() {}

void CRC32Handler::processReceivedMessage(protocol::p2pMessage& message, MessageProps& message_props) {
	if (!message_props.changed && !m_generator.checkMessageCRC32(message)) {
		std::clog << "[CRC32Handler] Rejected message: CRC-32 mismatch." << std::endl;

		message_props.repeat = false;
		message_props.skip = true;
	}
}

void CRC32Handler::processSentMessage(protocol::p2pMessage& message, MessageProps& message_props) {
}

} /* namespace handlers */
} /* namespace messaging */
} /* namespace p2pnet */
