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

#include "AgreementHandler.h"

namespace p2pnet {
namespace messaging {
namespace handlers {

AgreementHandler::AgreementHandler(MessageSocket* socket_ptr) : MessageHandler(socket_ptr) {
	m_netdb_storage = databases::NetDBStorage::getInstance();
}
AgreementHandler::~AgreementHandler() {}

std::string AgreementHandler::getHandlerName() {
	return "AgreementHandler";
}

void AgreementHandler::processReceivedMessage(protocol::p2pMessage& message, MessageState& message_state) {
}

void AgreementHandler::processSentMessage(protocol::p2pMessage& message, MessageState& message_state) {
}

} /* namespace handlers */
} /* namespace messaging */
} /* namespace p2pnet */
