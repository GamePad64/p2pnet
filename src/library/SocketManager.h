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

#include <boost/asio.hpp>
#include <thread>
#include "daemon_comm/APIClient.h"

namespace p2pnet {

/**
 * This class is used for connection to p2pnetd.
 */
class SocketManager {
	std::thread socket_thread;
	boost::asio::io_service* m_io_service;
	bool m_external_io_service;

	api::APIClient* m_primary_api_client;
public:
	SocketManager();
	SocketManager(boost::asio::io_service& io_service);
	virtual ~SocketManager();


};

} /* namespace p2pnet */

#endif /* SOCKETMANAGER_H_ */
