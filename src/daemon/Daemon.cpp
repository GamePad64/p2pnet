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

Daemon::Daemon() {
	m_netdb_storage = databases::NetDBStorage::getInstance();
	m_pk_storage = databases::PersonalKeyStorage::getInstance();
	m_sessionstorage = messaging::SessionStorage::getInstance();
	m_transport_socket = transport::TransportSocket::getInstance();
}
Daemon::~Daemon() {
	m_transport_socket->clear();
	m_sessionstorage->clear();
	m_pk_storage->clear();
	m_netdb_storage->clear();
}

int Daemon::run() {
	this->initTransportSocket();
	this->initLPD();
	AsioIOService::getIOService().run();
	return 0;
}

void Daemon::initTransportSocket() {
	// Creating TransportSocket
	m_transport_socket = transport::TransportSocket::getInstance();

	// Creating interfaces
	if(config_manager.getValue<bool>("transport.udp.enabled")){
		try {
			auto m_udp_interface = std::make_shared<transport::UDPTransportInterface>();
			m_transport_socket->registerInterface(m_udp_interface);
		} catch (boost::system::system_error& e) {
			log() << "Unable to initialize UDP socket. Exception caught: " << e.what() << std::endl;
		}
	}

	m_transport_socket->receive();
}

void Daemon::initLPD() {
	if(config_manager.getValue<bool>("discovery.udpv4.enabled")){
		try {
			discovery_udpv4 = new discovery::UDPLPDv4(config_manager);

			discovery_udpv4->startReceiveLoop();
			discovery_udpv4->startSendLoop();
		} catch (boost::system::system_error& e) {
			log(ERROR) << "Unable to initialize UDPv4 multicast LPD. Exception caught: " << e.what()
						<< std::endl;
		}
	}

	if(config_manager.getValue<bool>("discovery.udpv6.enabled")){
		try {
			discovery_udpv6 = new discovery::UDPLPDv6(config_manager);

			discovery_udpv6->startReceiveLoop();
			discovery_udpv6->startSendLoop();
		} catch (boost::system::system_error& e) {
			log(ERROR) << "Unable to initialize UDPv6 multicast LPD. Exception caught: " << e.what()
						<< std::endl;
		}
	}
}

} /* namespace p2pnet */
