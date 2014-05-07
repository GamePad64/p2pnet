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
#ifndef P2PUNIXDAEMON_H_
#define P2PUNIXDAEMON_H_

#include "../../common/api/UnixAPISocket.h"
#include "../P2PDaemon.h"

#include <thread>

namespace p2pnet {

class P2PUnixDaemon : public P2PDaemon {
	std::string m_socket_path;
	api::unix::UnixAPISocket* m_socket;

	bool connected;
public:
	P2PUnixDaemon();
	P2PUnixDaemon(std::shared_ptr<P2PSession> session);
	virtual ~P2PUnixDaemon();

	virtual void asyncSend(api::APIMessage data, api::SendHandler handler);
	virtual void asyncReceive(api::ReceiveHandler handler);

	void connect();
	void connect(std::string path);
	bool is_connected();
	int disconnect();
};

} /* namespace p2pnet */

#endif /* P2PUNIXDAEMON_H_ */
