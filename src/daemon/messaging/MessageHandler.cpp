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

#include "MessageHandler.h"

namespace p2pnet {
namespace messaging {

crypto::hash_t MessageHandler::getSourceTH(const protocol::p2pMessage& message) {
	protocol::p2pMessageHeader message_header;
	message_header = message.message_header();
	return crypto::hash_t(message_header.src_id().begin(), message_header.src_id().end());
}

} /* namespace messaging */
} /* namespace p2pnet */
