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
#ifndef APISESSION_H_
#define APISESSION_H_

#include "APIMessage.pb.h"

#include "../../common/Loggable.h"
#include "../../common/crypto/PrivateKeyDSA.h"

#include <boost/noncopyable.hpp>

namespace p2pnet {
namespace p2p {class Socket;class Node;}
namespace api {

class APIServer;

class APISession : public Loggable, public boost::noncopyable {
	struct node_t {	// We need to store it somewhere
		p2p::Node* node;
		uint32_t next_socket_id;
		std::map<uint32_t, p2p::Socket*> sockets;
	};
	std::map<uint32_t, node_t> nodes;
	uint32_t next_node_id;

	/**
	 * Throws std::system_error on client. It is thrown in answer to request_message.
	 * @param request_message
	 * @param error
	 */
	void remoteThrow(APIMessage request_message, std::error_condition error);

	// Functions that respond to specific MessageType of APIMessage.
	void NodeRegister(APIMessage request_message);
	void NodeUnRegister(APIMessage request_message);
	void NodeConnect(APIMessage request_message);
	void NodeAccept(APIMessage request_message);
	void NodeListen(APIMessage request_message);
	void NodeBind(APIMessage request_message);
	void SocketUnRegister(APIMessage request_message);

protected:
	APISession(APIServer* parent);

	APIServer* parent_api_server;
public:
	virtual ~APISession();

	virtual void send(APIMessage reply_message) = 0;
	void process(APIMessage request_message);

	void dropSession();	// Just a caution: deletes this APISession instance;
};

} /* namespace api */
} /* namespace p2pnet */

#endif /* APISESSION_H_ */
