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
#ifndef UNIXAPISERVER_H_
#define UNIXAPISERVER_H_

#include "APISession.h"
#include "APIServer.h"
#include "../../common/Config.h"

#include "../../common/api/UnixAPISocket.h"

#include <boost/asio.hpp>
#include <set>

#define MESSAGE_SIZE_TYPE uint32_t
#define MESSAGE_SIZE_LENGTH sizeof(MESSAGE_SIZE_TYPE)

using boost::asio::local::stream_protocol;

namespace p2pnet {
namespace api {
namespace unix {

class UnixAPIServer;

class UnixAPISession : public APISession {
	UnixAPISocket* m_unix_socket;
public:
	UnixAPISession(UnixAPISocket* preallocated_unix_socket, UnixAPIServer* server);
	virtual ~UnixAPISession();

	void send(APIMessage message);

	UnixAPISocket& getUnixSocket(){return *m_unix_socket;};
};

class UnixAPIServer : public APIServer, ConfigClient, Loggable {
	std::unique_ptr<stream_protocol::acceptor> acceptor_ptr;

	std::string socket_path;

	boost::asio::io_service& asio_io_service;

	void receiveLoop(UnixAPISession* session);
public:
	/**
	 * Constructs and starts Unix Domain Socket API server.
	 * @param io_service
	 * @throws boost::system::system_error	Constructor may throw on any Unix sockets error, starting from permission error to IO error.
	 */
	UnixAPIServer(boost::asio::io_service& io_service);
	virtual ~UnixAPIServer();

	void accept();
	void handleAccept(UnixAPISocket* new_socket);

	void handleReceive(UnixAPISession* new_session, api::APIMessage message, std::error_condition& error_condition);
};

} /* namespace unix */
} /* namespace api */
} /* namespace p2pnet */

#endif /* UNIXAPISERVER_H_ */
