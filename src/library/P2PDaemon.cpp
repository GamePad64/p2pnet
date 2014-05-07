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
#include "P2PDaemon.h"
#include "P2PSession.h"

namespace p2pnet {

P2PDaemon::P2PDaemon() {
	session = std::make_shared<P2PSession>(this);
}

P2PDaemon::P2PDaemon(std::shared_ptr<P2PSession> session){
	this->session = session;
}

void P2PDaemon::receiveLoop() {
	session->receiveLoop();
}

api::APIMessage P2PDaemon::clientExchange(api::APIMessage request_message) {
	return session->clientExchange(request_message);
}

} /* namespace p2pnet */
