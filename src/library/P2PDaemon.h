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
#ifndef P2PDAEMON_H_
#define P2PDAEMON_H_

#include "../common/api/APIMessage.pb.h"
#include "../common/api/api_handlers.h"
#include "../p2pnet.h"

#include <future>

namespace p2pnet {

class P2PSession;
class P2PDaemon {
protected:
	std::shared_ptr<P2PSession> session;
public:
	P2PDaemon();
	/**
	 * For something like P2PAutoDaemon, where we need to use child daemons.
	 * It does not connect automatically. Needs to connect() explicitly.
	 * @param session
	 */
	P2PDaemon(std::shared_ptr<P2PSession> session);
	virtual ~P2PDaemon(){};

	// We use asynchronous I/O.
	virtual void asyncSend(api::APIMessage data, api::SendHandler handler) = 0;
	virtual void asyncReceive(api::ReceiveHandler handler) = 0;

	void receiveLoop();
	api::APIMessage clientExchange(api::APIMessage request_message);

	virtual void connect() = 0;
	virtual bool is_connected() = 0;
	virtual int disconnect() = 0;
};

} /* namespace p2pnet */

#endif /* P2PDAEMON_H_ */
