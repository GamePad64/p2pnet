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
#ifndef CONNECTIONMANAGER_H_
#define CONNECTIONMANAGER_H_

#include "SH.h"

#include <map>
#include <set>

namespace p2pnet {
namespace api {class APISession;}
namespace p2p {

class LocalSocket;

class ConnectionManager {
	friend class LocalSocket;

	std::map<SH, LocalSocket*> bound_sockets;
	std::set<LocalSocket*> not_bound_sockets;
public:
	ConnectionManager();
	virtual ~ConnectionManager();

	LocalSocket* createSocket();
	LocalSocket* createSocket(api::APISession* api_session);

	void destroySocket(LocalSocket* socket_ptr);
};

} /* namespace p2p */
} /* namespace p2pnet */

#endif /* CONNECTIONMANAGER_H_ */
