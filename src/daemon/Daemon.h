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
#include "AsioIOService.h"

#include "databases/NetDBStorage.h"
#include "databases/PersonalKeyStorage.h"
#include "messaging/SessionMap.h"

#include "messaging/MessageSocket.h"

#include "net/TransportSocket.h"

#include "net/udp/UDPTransportInterface.h"
#include "net/lpd/UDPLPDv4.h"
#include "net/lpd/UDPLPDv6.h"

namespace p2pnet {

class Daemon {
	void initTransportSocket();
	void initLPD();
public:
	ConfigManager config_manager;

	databases::NetDBStorage* m_netdb_storage;
	databases::PersonalKeyStorage* m_pk_storage;
	messaging::SessionMap* m_sessionmap;

	// TransportSocket with its interfaces.
	net::TransportSocket* m_transport_socket;
	net::UDPTransportInterface* m_udp_interface;

	// MessageSocket
	messaging::MessageSocket* m_message_socket;

	net::lpd::UDPLPDv4* m_lpd_udpv4;
	net::lpd::UDPLPDv6* m_lpd_udpv6;

	Daemon();
	virtual ~Daemon();

	void run();
};

} /* namespace p2pnet */
#endif /* DAEMON_H_ */
