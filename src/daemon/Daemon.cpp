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
#include "net/udp/UDPTransportInterfaceEndpoint.h"
#include "net/lpd/UDPLPDv4.h"

namespace p2pnet {

Daemon::Daemon() {
	m_netdb_storage = databases::NetDBStorage::getInstance();
	m_pk_storage = databases::PersonalKeyStorage::getInstance();
	m_sessionmap = messaging::SessionStorage::getInstance();
}
Daemon::~Daemon() {
	databases::NetDBStorage::clear();
	databases::PersonalKeyStorage::clear();
	messaging::SessionStorage::clear();

	if (m_transport_socket) delete m_transport_socket;

	if (m_lpd_udpv4) delete m_lpd_udpv4;
	if (m_lpd_udpv6) delete m_lpd_udpv6;
}

void Daemon::run() {
	this->initTransportSocket();
	this->initMessageSocket();
	this->initLPD();
	AsioIOService::getIOService().run();
}

void Daemon::initTransportSocket() {
	try {
		m_transport_socket = net::TransportSocket::getInstance();
		m_transport_socket->addListener(&m_message_socket);

		m_udp_interface = new net::UDPTransportInterface();
		unsigned short int port_v6 = config.getConfig().get("net.sockets.udpv6.port", 2185);
		m_udp_interface->bindLocalAll(port_v6);

		net::UDPTransportInterfaceEndpoint endpoint(config.getConfig().get("net.sockets.udpv6.bind", "0::0"), port_v6);
		m_transport_socket->registerInterface(m_udp_interface);

		m_udp_interface->asyncReceiveFrom(std::make_shared<net::UDPTransportInterfaceEndpoint>(endpoint));
	} catch (boost::system::system_error& e) {
		std::clog << "[Daemon] Unable to initialize IPv6 UDP socket. Exception caught: " << e.what() << std::endl;
	}
}

void Daemon::initMessageSocket() {

}

void Daemon::initLPD() {
	try {
		m_lpd_udpv4 = new net::lpd::UDPLPDv4(config);
		m_lpd_udpv4->startReceive();
		m_lpd_udpv4->startSend();
	} catch (boost::system::system_error& e) {
		std::clog << "[Daemon] Unable to initialize IPv4 UDP multicast LPD. Exception caught: " << e.what()
				<< std::endl;
	}

	try {
		m_lpd_udpv6 = new net::lpd::UDPLPDv6(config);
		m_lpd_udpv6->startReceive();
		m_lpd_udpv6->startSend();
	} catch (boost::system::system_error& e) {
		std::clog << "[Daemon] Unable to initialize IPv6 UDP multicast LPD. Exception caught: " << e.what()
				<< std::endl;
	}
}

} /* namespace p2pnet */
