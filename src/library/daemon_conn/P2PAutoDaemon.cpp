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
#include "P2PAutoDaemon.h"

namespace p2pnet {

P2PAutoDaemon::P2PAutoDaemon() {
	underlying_daemon = new P2PUnixDaemon(session);
	connect();
}

P2PAutoDaemon::P2PAutoDaemon(std::shared_ptr<P2PSession> session) : P2PDaemon(session) {
	underlying_daemon = new P2PUnixDaemon(session);
}

P2PAutoDaemon::~P2PAutoDaemon() {
	delete underlying_daemon;
}

void P2PAutoDaemon::asyncSend(api::APIMessage data, api::SendHandler handler) {
	underlying_daemon->asyncSend(data, handler);
}

void P2PAutoDaemon::asyncReceive(api::ReceiveHandler handler) {
	underlying_daemon->asyncReceive(handler);
}

void P2PAutoDaemon::connect() {
	underlying_daemon->connect();
}

bool P2PAutoDaemon::is_connected() {
	return underlying_daemon->is_connected();
}

int P2PAutoDaemon::disconnect() {
	return underlying_daemon->disconnect();
}

} /* namespace p2pnet */
