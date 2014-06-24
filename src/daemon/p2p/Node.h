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

#include "../../common/Config.h"

#include <boost/optional.hpp>
#include <deque>

namespace p2pnet {
namespace api {class APISession;}
namespace p2p {

class Node : public ConfigClient {
	crypto::PrivateKeyDSA bound_private_key;
	SH bound_sh;
	api::APISession* api_session;

	uint32_t max_connections = 0;
	boost::optional<bool> loopback_param;

	std::map<SH, Socket*> connected_sockets;

	std::deque<Connection*> incoming_connections;
public:
	Node();
	Node(api::APISession* api_session);
	virtual ~Node();

	bool isBound();
	SH getLocalSH();

	void bind(crypto::PrivateKeyDSA private_key);
	void listen(uint32_t max_connections);

	void connect(SH sh);
	void accept();

	bool loopback();
	void loopback(bool set_loopback);
	void resetLoopback();
};

} /* namespace p2p */
} /* namespace p2pnet */

#endif /* NODE_H_ */
