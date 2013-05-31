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
#include <iostream>

namespace p2pnet {
namespace messaging {
namespace handlers {

std::string MessageHandler::getHandlerName(){
	return "MessageHandler";
}

void MessageHandler::complete(MessageState& message_state) {
	message_state.repeat = false;
	message_state.skip = true;
}

void MessageHandler::reject(Reason reason) {
	throw(new RejectException(reason, getHandlerName()));
}

void MessageHandler::reject(Reason reason, std::string comment) {
	throw(new RejectException(reason, comment, getHandlerName()));
}

}
}
}
