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

#include "transport/udp/UDPTransportInterface.h"
#include "Daemon.h"

namespace p2pnet {

Daemon::Daemon() {
	config_manager = ConfigManager::getInstance();
	m_pk_storage = databases::PersonalKeyStorage::getInstance();
	m_transport_socket = transport::TransportSocket::getInstance();
	m_overlay_socket = overlay::OverlaySocket::getInstance();
}
Daemon::~Daemon() {
	m_overlay_socket->clear();
	m_transport_socket->clear();
	m_pk_storage->clear();
	config_manager->clear();
}

int Daemon::run() {
	this->initTransportSocket();
	this->initDiscoveryServices();
	this->initAPI();

	AsioIOService::getIOService().run();
	return 0;
}

void Daemon::initAPI() {
	api_manager = std::unique_ptr<api::APIManager>(new api::APIManager());
}

void Daemon::initTransportSocket() {
	// Creating TransportSocket
	m_transport_socket = transport::TransportSocket::getInstance();

	// Creating interfaces
	if(config_manager->getValue<bool>("transport.udp.enabled")){
		try {
			auto m_udp_interface = std::make_shared<transport::UDPTransportInterface>();
			m_transport_socket->registerInterface(m_udp_interface);
		} catch (boost::system::system_error& e) {
			log() << "Unable to initialize UDP socket. Exception caught: " << e.what() << std::endl;
		}
	}

	m_transport_socket->receive();
}

void Daemon::initDiscoveryServices() {
	if(config_manager->getValue<std::string>("discovery.bootstrap.filename") != ""){
		discovery_bootstrap = std::unique_ptr<discovery::BootstrapDiscovery>(new discovery::BootstrapDiscovery());
		discovery_bootstrap->run();
	}

	if(config_manager->getValue<bool>("discovery.udpv4.enabled")){
		try {
			discovery_udpv4 = std::unique_ptr<discovery::UDPLPDv4>(new discovery::UDPLPDv4());

			discovery_udpv4->startReceiveLoop();
			discovery_udpv4->startSendLoop();
		} catch (boost::system::system_error& e) {
			log(ERROR) << "Unable to initialize UDPv4 multicast LPD. Exception caught: " << e.what()
						<< std::endl;
		}
	}

	if(config_manager->getValue<bool>("discovery.udpv6.enabled")){
		try {
			discovery_udpv6 = std::unique_ptr<discovery::UDPLPDv6>(new discovery::UDPLPDv6());

			discovery_udpv6->startReceiveLoop();
			discovery_udpv6->startSendLoop();
		} catch (boost::system::system_error& e) {
			log(ERROR) << "Unable to initialize UDPv6 multicast LPD. Exception caught: " << e.what()
						<< std::endl;
		}
	}
}

} /* namespace p2pnet */
