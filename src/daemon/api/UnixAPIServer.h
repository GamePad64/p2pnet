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

#include "../../library/export.h"

#include <boost/asio.hpp>
#include <set>

#define MESSAGE_SIZE_TYPE uint32_t
#define MESSAGE_SIZE_LENGTH sizeof(MESSAGE_SIZE_TYPE)

using boost::asio::local::stream_protocol;

namespace p2pnet {
namespace api {
namespace unix {

class UnixAPIServer;

class UnixAPISession : public APISession, public std::enable_shared_from_this<UnixAPISession> {
	UnixAPISocket socket;
	UnixAPIServer* parent_apiserver;
public:
	UnixAPISession(boost::asio::io_service& io_service, UnixAPIServer* parent);
	virtual ~UnixAPISession();

	void send(APIMessage message);
	//void process(APIMessage message); <--- Inherited from APISession.

	void shutdown();	// Careful! Acts like `delete this;`

	UnixAPISocket& getUnixSocket(){return socket;}

	std::string getComponentName() const {
		return "UnixAPISession";
	}
};

class UnixAPIServer : public APIServer, ConfigClient, Loggable {
	friend class UnixAPISession;
	std::set<std::shared_ptr<UnixAPISession>> unix_sessions;

	std::unique_ptr<stream_protocol::acceptor> acceptor_ptr;

	std::string socket_path;

	boost::asio::io_service& asio_io_service;
public:
	/**
	 * Constructs and starts Unix Domain Socket API server.
	 * @param io_service
	 * @throws boost::system::system_error	Constructor may throw on any Unix sockets error, starting from permission error to IO error.
	 */
	UnixAPIServer(boost::asio::io_service& io_service);
	virtual ~UnixAPIServer();

	void accept();
	void handleAccept(std::shared_ptr<UnixAPISession> new_session);

	std::string getComponentName() {
		return "UnixAPIServer";
	}
};

} /* namespace unix */
} /* namespace api */
} /* namespace p2pnet */

#endif /* UNIXAPISERVER_H_ */
