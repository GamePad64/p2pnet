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
#ifndef P2PAUTODAEMON_H_
#define P2PAUTODAEMON_H_

#include "../P2PDaemon.h"
#include "P2PUnixDaemon.h"

namespace p2pnet {

class P2PAutoDaemon : public P2PDaemon {
	P2PDaemon* underlying_daemon;
public:
	P2PAutoDaemon();
	P2PAutoDaemon(std::shared_ptr<P2PSession> session);
	virtual ~P2PAutoDaemon();

	virtual void asyncSend(api::APIMessage data, api::SendHandler handler);
	virtual void asyncReceive(api::ReceiveHandler handler);

	virtual void connect();
	virtual bool is_connected();
	virtual int disconnect();
};

} /* namespace p2pnet */

#endif /* P2PAUTODAEMON_H_ */
