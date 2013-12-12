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
#ifndef OVERLAYSOCKET_H_
#define OVERLAYSOCKET_H_

#include "TH.h"
#include "OverlayPeer.h"
#include "OverlayDHT.h"
#include "../transport/TransportSocketEndpoint.h"
#include "../../common/Singleton.h"
#include "../../common/Loggable.h"

namespace p2pnet {
namespace overlay {

class OverlayConnection;

class OverlaySocket : public Singleton<OverlaySocket>, Loggable {
	friend class OverlayPeer;
	friend class OverlayConnection;
	friend class OverlayDHT;
protected:
	OverlayDHT dht_service;

	struct overlay_peer_conn_t {
		std::shared_ptr<OverlayPeer> peer;
		std::shared_ptr<OverlayConnection> connection;
	};
	std::map<overlay::TH, overlay_peer_conn_t> m_peers_conns;
	//std::set<transport::TransportSocketEndpoint> banned_peer_list;	// For future use.

public:
	OverlaySocket();
	virtual ~OverlaySocket();

	std::string getComponentName(){return "OverlaySocket";}

	void send(const overlay::TH& dest,
			const protocol::OverlayMessage_Payload& message_payload,
			protocol::OverlayMessage_Header_MessagePriority prio);
	void process(std::string data, const transport::TransportSocketEndpoint& from);

	std::shared_ptr<OverlayConnection> getConnection(const overlay::TH& th);
	std::shared_ptr<OverlayPeer> getPeer(const overlay::TH& th);
	void removePeer(const overlay::TH& th);
	void movePeer(const overlay::TH& from, const overlay::TH& to);
};

} /* namespace overlay */
} /* namespace p2pnet */

#endif /* OVERLAYSOCKET_H_ */
