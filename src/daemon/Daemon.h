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

#include "../common/Config.h"
#include "../common/Loggable.h"
#include "AsioIOService.h"

#include "databases/PersonalKeyStorage.h"

#include "api/APIManager.h"

#include "transport/TransportSocket.h"
#include "overlay/OverlaySocket.h"

#include "discovery/BootstrapDiscovery.h"
#include "discovery/UDPLPDv4.h"
#include "discovery/UDPLPDv6.h"

namespace p2pnet {

class Daemon : public Loggable {
	void initAPI();
	void initTransportSocket();
	void initDiscoveryServices();
public:
	ConfigManager* config_manager;

	databases::PersonalKeyStorage* m_pk_storage;

	transport::TransportSocket* m_transport_socket;
	overlay::OverlaySocket* m_overlay_socket;

	std::unique_ptr<discovery::BootstrapDiscovery> discovery_bootstrap;
	std::unique_ptr<discovery::UDPLPDv4> discovery_udpv4;
	std::unique_ptr<discovery::UDPLPDv6> discovery_udpv6;

	std::unique_ptr<api::APIManager> api_manager;

	Daemon();
	virtual ~Daemon();

	int run();
};

} /* namespace p2pnet */
#endif /* DAEMON_H_ */
