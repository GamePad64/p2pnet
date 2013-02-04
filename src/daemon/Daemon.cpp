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

#include "Daemon.h"

namespace p2pnet {

Daemon::Daemon() {}
Daemon::~Daemon() {}

void Daemon::run(){

}

void Daemon::initializeSockets() {
	udpv4_socket.bindLocalIPv4(config.net_v4_udp_port);
	udpv4_socket.addListener(&message_dispatcher);
	udpv4_socket.

	udpv6_socket.bindLocalIPv6(config.net_v6_udp_port);
	udpv6_socket.addListener(&message_dispatcher);
}

void Daemon::runIOService() {
	AsioIOService::getIOService().run();
}

} /* namespace p2pnet */