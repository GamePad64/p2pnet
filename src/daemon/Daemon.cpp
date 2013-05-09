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
#include "net/udp/UDPTransportSocketEndpoint.h"
#include "net/lpd/UDPLPDv4.h"

namespace p2pnet {

Daemon::Daemon() : crc32_handler(&m_message_socket),
		relay_handler(&m_message_socket),
		m_lpd_udpv4(config, m_transport_socket_udpv4),
		m_lpd_udpv6(config, m_transport_socket_udpv6){
	m_netdb_storage = databases::NetDBStorage::getInstance();
	m_pk_storage = databases::PersonalKeyStorage::getInstance();
}
Daemon::~Daemon() {
	databases::NetDBStorage::clear();
	databases::PersonalKeyStorage::clear();
}

void Daemon::run(){
	this->initTransportSockets();
	this->initMessageSocket();
	this->initLPD();
	AsioIOService::getIOService().run();
}

void Daemon::initTransportSockets() {
	if(config.getConfig().get("net.sockets.udpv4.enable", true)){
		try {
			unsigned short int port_v4 = config.getConfig().get("net.sockets.udpv4.port", 2185);
			m_transport_socket_udpv4.bindLocalIPv4(port_v4);

			// Here we add listeners to sockets. Basically, when we expand our protocol, we should add new listeners.
			m_transport_socket_udpv4.addListener(&m_message_socket);

			net::UDPTransportSocketEndpoint endpoint(config.getConfig().get("net.sockets.udpv4.bind", "0.0.0.0"), port_v4);
			m_transport_socket_udpv4.asyncReceiveFrom(endpoint);
		} catch(boost::system::system_error& e) {
			std::clog << "[Daemon] Unable to initialize IPv4 UDP socket. Exception caught: " << e.what() << std::endl;
		}
	}

	if(config.getConfig().get("net.sockets.udpv6.enable", true)){
		try {
			unsigned short int port_v6 = config.getConfig().get("net.sockets.udpv6.port", 2185);
			m_transport_socket_udpv6.bindLocalIPv6(port_v6);


			m_transport_socket_udpv6.addListener(&m_message_socket);

			net::UDPTransportSocketEndpoint endpoint(config.getConfig().get("net.sockets.udpv6.bind", "0::0"), port_v6);
			m_transport_socket_udpv6.asyncReceiveFrom(endpoint);
		} catch(boost::system::system_error& e) {
			std::clog << "[Daemon] Unable to initialize IPv6 UDP socket. Exception caught: " << e.what() << std::endl;
		}
	}
}

void Daemon::initMessageSocket(){
	m_message_socket.addHandler(&crc32_handler);
	m_message_socket.addHandler(&relay_handler);
}

void Daemon::initLPD() {
	m_lpd_udpv4.startReceive();
	m_lpd_udpv6.startReceive();

	m_lpd_udpv4.startSend();
	m_lpd_udpv6.startSend();
}

} /* namespace p2pnet */
