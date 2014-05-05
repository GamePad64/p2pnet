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
#ifndef API_HANDLERS_H_
#define API_HANDLERS_H_

#include "APIMessage.pb.h"

#include <system_error>
#include <functional>

namespace p2pnet {
namespace api {

typedef std::function<void(std::error_condition&)> SendHandler;
typedef std::function<void(api::APIMessage, std::error_condition&)> ReceiveHandler;

} /* namespace api */
} /* namespace p2pnet */

#endif /* API_HANDLERS_H_ */
