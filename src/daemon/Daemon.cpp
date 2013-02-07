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
#include "net/UDPTransportSocketEndpoint.h"

namespace p2pnet {

Daemon::Daemon() {}
Daemon::~Daemon() {}

void Daemon::run(){

}

void Daemon::initializeSockets() {
	if(config.getConfig().get("net.udp.v4.enable", true)){
		udpv4_socket.bindLocalIPv4( static_cast<unsigned short int>(config.getConfig().get("net.udp.v4.port", 2185)) );
		udpv4_socket.addListener(&message_dispatcher);

		net::UDPTransportSocketEndpoint endpoint(config.getConfig().get("net.udp.v4.bind", "0.0.0.0"), config.getConfig().get("net.udp.v4.port", 2185));
		udpv4_socket.asyncReceiveFrom(endpoint);
	}

	if(config.getConfig().get("net.udp.v6.enable", true)){
		udpv6_socket.bindLocalIPv6( static_cast<unsigned short int>(config.getConfig().get("net.udp.v6.port", 2185)) );
		udpv6_socket.addListener(&message_dispatcher);

		net::UDPTransportSocketEndpoint endpoint(config.getConfig().get("net.udp.v6.bind", "0::0"), config.getConfig().get("net.udp.v6.port", 2185));
		udpv6_socket.asyncReceiveFrom(endpoint);
	}
}

void Daemon::runIOService() {
	AsioIOService::getIOService().run();
}

} /* namespace p2pnet */
