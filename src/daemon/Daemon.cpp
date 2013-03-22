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
#include "net/lpd/UDPLPDv4.h"

namespace p2pnet {

Daemon::Daemon() : m_lpd_udpv4(config, m_socket_udpv4, m_netdb_storage){
	m_pk_storage = databases::PersonalKeyStorage::getInstance();
}
Daemon::~Daemon() {
	delete m_pk_storage;
}

void Daemon::run(){
	this->initializeSockets();
	this->runLPD();
	AsioIOService::getIOService().run();
}

void Daemon::initializeSockets() {
	if(config.getConfig().get("net.sockets.udpv4.enable", true)){
		try {
			m_socket_udpv4.bindLocalIPv4( static_cast<unsigned short int>(config.getConfig().get("net.sockets.udpv4.port", 2185)) );
			m_socket_udpv4.addListener(&message_dispatcher);

			net::UDPTransportSocketEndpoint endpoint(config.getConfig().get("net.sockets.udpv4.bind", "0.0.0.0"), config.getConfig().get("net.sockets.udpv4.port", 2185));
			m_socket_udpv4.asyncReceiveFrom(endpoint);
		} catch(boost::system::system_error& e) {
			std::clog << "[Daemon] Unable to initialize IPv4 UDP socket. Exception caught: " << e.what() << std::endl;
		}
	}

	if(config.getConfig().get("net.sockets.udpv6.enable", true)){
		try {
			m_socket_udpv6.bindLocalIPv6( static_cast<unsigned short int>(config.getConfig().get("net.sockets.udpv6.port", 2185)) );
			m_socket_udpv6.addListener(&message_dispatcher);

			net::UDPTransportSocketEndpoint endpoint(config.getConfig().get("net.sockets.udpv6.bind", "0::0"), config.getConfig().get("net.sockets.udpv6.port", 2185));
			m_socket_udpv6.asyncReceiveFrom(endpoint);
		} catch(boost::system::system_error& e) {
			std::clog << "[Daemon] Unable to initialize IPv6 UDP socket. Exception caught: " << e.what() << std::endl;
		}
	}
}

void Daemon::runLPD() {
	m_lpd_udpv4.startReceive();
	m_lpd_udpv4.startSend();
}

} /* namespace p2pnet */
