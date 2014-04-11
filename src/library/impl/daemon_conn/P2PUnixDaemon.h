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

#include "../../../common/api/UnixAPISocket.h"
#include "../P2PDaemon.h"

#include <thread>

namespace p2pnet {

class P2PUnixDaemon : public P2PDaemon {
	std::string m_socket_path;
	api::unix::UnixAPISocket* m_socket;

	std::thread* socket_thread;
	boost::asio::io_service* m_io_service;
	bool m_external_io_service;
	bool connected;
public:
	P2PUnixDaemon();
	P2PUnixDaemon(boost::asio::io_service& io_service);
	virtual ~P2PUnixDaemon();

	void send(api::APIMessage data, int& error_code);
	api::APIMessage receive(int& error_code);

	int connect();
	int connect(std::string path);
	bool is_connected();
	int disconnect();
};

} /* namespace p2pnet */

#endif /* P2PUNIXDAEMON_H_ */
