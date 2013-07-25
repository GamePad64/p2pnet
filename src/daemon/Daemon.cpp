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

namespace p2pnet {

Daemon::Daemon() {
	m_netdb_storage = databases::NetDBStorage::getInstance();
	m_pk_storage = databases::PersonalKeyStorage::getInstance();
	m_sessionmap = messaging::SessionStorage::getInstance();
	m_message_socket = messaging::MessageSocket::getInstance();
}
Daemon::~Daemon() {
	databases::NetDBStorage::clear();
	databases::PersonalKeyStorage::clear();
	messaging::SessionStorage::clear();
	messaging::MessageSocket::clear();

	if (m_transport_socket) delete m_transport_socket;

	if (m_lpd_udpv4) delete m_lpd_udpv4;
	if (m_lpd_udpv6) delete m_lpd_udpv6;
}

void Daemon::run() {
	this->initTransportSocket();
	this->initLPD();
	AsioIOService::getIOService().run();
}

void Daemon::initTransportSocket() {
	// Creating TransportSocket
	m_transport_socket = net::TransportSocket::getInstance();
	m_transport_socket->addListener(m_message_socket);

	// Creating interfaces
	m_udp_interface = new net::UDPTransportInterface();

	// Registering interfaces in TransportSocket
	m_transport_socket->registerInterface(m_udp_interface);

	// Configuring interfaces
	try {
		unsigned short int port = config_manager.getConfigValueOrDefault<unsigned short int>("net.sockets.udp.port");
		m_udp_interface->bindLocalAll(port);

		// Begin receiving from UDP
		net::UDPTransportInterfaceEndpoint endpoint(config_manager.getConfigValueOrDefault<std::string>("net.sockets.udp.local_ip"), port);
		m_udp_interface->asyncReceiveFrom(std::make_shared<net::UDPTransportInterfaceEndpoint>(endpoint));
	} catch (boost::system::system_error& e) {
		std::clog << "[Daemon] Unable to initialize UDP socket. Exception caught: " << e.what() << std::endl;
	}
}

void Daemon::initLPD() {
	m_lpd_udpv4 = new net::lpd::UDPLPDv4(config_manager);
	m_lpd_udpv6 = new net::lpd::UDPLPDv6(config_manager);

	try {
		m_lpd_udpv4->startReceive();
		m_lpd_udpv4->startSend();
	} catch (boost::system::system_error& e) {
		std::clog << "[Daemon] Unable to initialize UDPv4 multicast LPD. Exception caught: " << e.what()
				<< std::endl;
	}
	try {
		m_lpd_udpv6->startReceive();
		m_lpd_udpv6->startSend();
	} catch (boost::system::system_error& e) {
		std::clog << "[Daemon] Unable to initialize UDPv6 multicast LPD. Exception caught: " << e.what()
				<< std::endl;
	}
}

} /* namespace p2pnet */
