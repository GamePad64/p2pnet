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
}
Daemon::~Daemon() {
	databases::NetDBStorage::clear();
	databases::PersonalKeyStorage::clear();
	messaging::SessionStorage::clear();

	if (m_transport_socket) delete m_transport_socket;

	if (m_lpd_udp) delete m_lpd_udp;
}

void Daemon::run() {
	this->initTransportSocket();
	this->initMessageSocket();
	this->initLPD();
	AsioIOService::getIOService().run();
}

void Daemon::initTransportSocket() {
	// Creating TransportSocket
	m_transport_socket = net::TransportSocket::getInstance();
	m_transport_socket->addListener(&m_message_socket);

	// Creating interfaces
	m_udp_interface = new net::UDPTransportInterface();

	// Registering interfaces in TransportSocket
	m_transport_socket->registerInterface(m_udp_interface);

	// Configuring interfaces
	try {
		unsigned short int port = config_manager.getConfig().get("net.sockets.udp.port", 2185);
		m_udp_interface->bindLocalAll(port);

		// Begin receiving from UDP
		net::UDPTransportInterfaceEndpoint endpoint(config_manager.getConfig().get("net.sockets.udp.local_ip", "0::0"), port);
		m_udp_interface->asyncReceiveFrom(std::make_shared<net::UDPTransportInterfaceEndpoint>(endpoint));
	} catch (boost::system::system_error& e) {
		std::clog << "[Daemon] Unable to initialize UDP socket. Exception caught: " << e.what() << std::endl;
	}
}

void Daemon::initMessageSocket() {
	m_transport_socket->addListener(&m_message_socket);
}

void Daemon::initLPD() {
	try {
		m_lpd_udp = new net::lpd::UDPLPD(config_manager);
		m_lpd_udp->startReceive();
		m_lpd_udp->startSend();
	} catch (boost::system::system_error& e) {
		std::clog << "[Daemon] Unable to initialize UDP multicast LPD. Exception caught: " << e.what()
				<< std::endl;
	}
}

} /* namespace p2pnet */
