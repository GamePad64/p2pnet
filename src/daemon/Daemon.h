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

#ifndef DAEMON_H_
#define DAEMON_H_

#include "AsioIOService.h"
#include "net/UDPTransportSocket.h"
#include "messaging/TransportSocketMessageDispatcher.h"
#include "../common/Config.h"

namespace p2pnet {

class Daemon {
	Config config;

	net::UDPTransportSocket udpv4_socket;
	net::UDPTransportSocket udpv6_socket;

	messaging::TransportSocketMessageDispatcher message_dispatcher;
public:
	Daemon();
	virtual ~Daemon();

	void run();
	void initializeSockets();
	void runIOService();
};

} /* namespace p2pnet */
#endif /* DAEMON_H_ */
