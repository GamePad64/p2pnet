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
#include "Connection.h"
#include "Interface.h"

namespace p2pnet {
namespace transport {

Connection::Connection(std::shared_ptr<Interface> parent_interface) : parent_interface(parent_interface) {}

void Connection::onConnect(){
	parent_interface.lock()->getParent()->registerConnection(shared_from_this());
}
void Connection::onDisconnect(){
	parent_interface.lock()->getParent()->unregisterConnection(shared_from_this());
}

} /* namespace transport */
} /* namespace p2pnet */
