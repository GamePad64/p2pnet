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
#include "P2PLocalDaemon.h"

namespace p2pnet {

P2PLocalDaemon::P2PLocalDaemon() {
	underlying_daemon = new P2PUnixDaemon();

	asyncReceive(std::bind(&P2PDaemon::handleReceive, this, std::placeholders::_1, std::placeholders::_2));	// Start receive loop.
}

P2PLocalDaemon::~P2PLocalDaemon() {
	delete underlying_daemon;
}

void P2PLocalDaemon::asyncSend(api::APIMessage data, SendHandler handler) {
	underlying_daemon->asyncSend(data, handler);
}

void P2PLocalDaemon::asyncReceive(ReceiveHandler handler) {
	underlying_daemon->asyncReceive(handler);
}

void P2PLocalDaemon::connect() {
	underlying_daemon->connect();
}

bool P2PLocalDaemon::is_connected() {
	return underlying_daemon->is_connected();
}

int P2PLocalDaemon::disconnect() {
	return underlying_daemon->disconnect();
}

} /* namespace p2pnet */
