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
#ifndef SOCKET_H_
#define SOCKET_H_

#include "SH.h"
#include "../../common/crypto/PublicKeyDSA.h"

namespace p2pnet {
namespace p2p {

class Node;
class Connection;

class Socket {
	Node* parent_node;

	Connection* connection;
public:
	Socket(Node* node, Connection* incoming_connection);	// Accept incoming connection
	Socket(Node* node, SH sh_to_connect);	// Connect to remote node
	virtual ~Socket();

	SH getRemoteSH();
};

} /* namespace p2p */
} /* namespace p2pnet */

#endif /* SOCKET_H_ */
