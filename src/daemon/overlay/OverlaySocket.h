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
#include "OverlayDHT.h"
#include "OverlayKeyProvider.h"
#include "OverlayNodeDB.h"

#include "../transport/TransportSocketEndpoint.h"
#include "../../common/Singleton.h"
#include "../../common/Loggable.h"

namespace p2pnet {
namespace overlay {

class OverlayConnection;

class OverlaySocket : public Singleton<OverlaySocket>, Loggable, ConfigClient {
	friend class OverlayConnection;
	friend class OverlayDHT;
protected:
	std::set<transport::TransportSocketEndpoint> banned_peer_list;

	OverlayKeyProvider key_provider;
	OverlayNodeDB node_db;
	OverlayDHT dht_service;

public:
	enum class Priority {
		REALTIME = protocol::OverlayMessage_Header_MessagePriority_REALTIME,
		RELIABLE = protocol::OverlayMessage_Header_MessagePriority_RELIABLE
	};

	OverlaySocket();
	virtual ~OverlaySocket();

	void send(const TH& dest,
			const protocol::OverlayMessage_Payload& message_payload, Priority prio);
	void process(std::string data, const transport::TransportSocketEndpoint& from);

	OverlayKeyProvider* getKeyProvider(){return &key_provider;};
	OverlayNodeDB* getNodeDB(){return &node_db;};
	OverlayDHT* getDHT(){return &dht_service;};
};

} /* namespace overlay */
} /* namespace p2pnet */

#endif /* OVERLAYSOCKET_H_ */
