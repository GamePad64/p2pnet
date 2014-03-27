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
#ifndef SOCKETMANAGER_H_
#define SOCKETMANAGER_H_

#include "daemon_comm/APIClient.h"
#include <boost/asio.hpp>
#include <thread>
#include <memory>

namespace p2pnet {
namespace impl {

/**
 * This class is used for connection to p2pnetd.
 *
 * It doesn't need to be created explicitly, P2PSocket's default constructor creates a singleton instance of it.
 * So, for writing applications that use only single default p2pnetd instance (and this is likely)
 * you don't need to bother with this class, just create P2PSocket.
 */
class P2PDaemonConnectionImpl {
	std::thread socket_thread;
	boost::asio::io_service* m_io_service;
	bool m_external_io_service;

	std::shared_ptr<api::APIClient> m_api_client;
	std::map<int, std::shared_ptr<P2PSocketImpl>> m_socket_ids;
public:
	P2PDaemonConnectionImpl(bool autoconnect = true);
	P2PDaemonConnectionImpl(boost::asio::io_service& io_service, bool autoconnect = true);
	virtual ~P2PDaemonConnectionImpl();

	int connect();
	bool is_connected();

	std::shared_ptr<api::APIClient> getAPIClient();
};

} /* namespace impl */
} /* namespace p2pnet */

#endif /* SOCKETMANAGER_H_ */
