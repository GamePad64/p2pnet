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
#ifndef NODE_H_
#define NODE_H_

#include "SH.h"
#include "../../common/crypto/PrivateKeyDSA.h"
#include "Socket.h"

namespace p2pnet {
namespace api {class APISession;}
namespace p2p {

/*
	std::unique_ptr<Connection> connection;
	SH bound_sh;
	api::APISession* api_session;
public:
	LocalSocket();
	LocalSocket(api::APISession* api_session);
	virtual ~LocalSocket();

	bool isBound();
	SH getLocalSH();

	bool loopback();
	void loopback(bool enabled);
*/

class Node {
	crypto::PrivateKeyDSA bound_private_key;
	SH bound_sh;
	api::APISession* api_session;

	uint32_t max_connections = 0;

	std::map<SH, Socket> connected_sockets;
public:
	Node();
	Node(api::APISession* api_session);
	virtual ~Node();

	bool isBound();
	SH getLocalSH();

	void bind(crypto::PrivateKeyDSA private_key);
	void listen(uint32_t max_connections);
};

} /* namespace p2p */
} /* namespace p2pnet */

#endif /* NODE_H_ */
