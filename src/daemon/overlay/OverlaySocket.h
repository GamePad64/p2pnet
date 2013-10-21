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
#include "../transport/TransportSocketEndpoint.h"
#include "../../common/Singleton.h"
#include "../../common/Loggable.h"

namespace p2pnet {
namespace overlay {

class OverlayConnection;

class OverlaySocket : public Singleton<OverlaySocket>, Loggable {
	friend class OverlayConnection;
protected:
	std::map<overlay::TH, std::shared_ptr<OverlayConnection>> m_connections;
	//std::set<transport::TransportSocketEndpoint> banned_peer_list;	// For future use.
	std::shared_ptr<OverlayConnection> addConnection(overlay::TH th);
public:
	OverlaySocket();
	virtual ~OverlaySocket();

	void send(overlay::TH dest, std::string data);
	void process(std::string data, transport::TransportSocketEndpoint from);
};

} /* namespace overlay */
} /* namespace p2pnet */

#endif /* OVERLAYSOCKET_H_ */
