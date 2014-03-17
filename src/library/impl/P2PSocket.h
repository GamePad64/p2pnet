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
#ifndef P2PSOCKET_H_
#define P2PSOCKET_H_

#include "../common/crypto/PrivateKeyDSA.h"
#include "P2PAssociation.h"
#include "DaemonConnectionImpl.h"

namespace p2pnet {

class P2PSocket {
	DaemonConnectionImpl* m_parent_socket_manager;
	std::shared_ptr<api::APIClient> m_api_client_ptr;

	uint32_t socket_id;

	crypto::PrivateKeyDSA m_bound_private_key;
public:
	P2PSocket();
	P2PSocket(DaemonConnectionImpl& parent_socket_manager);
	P2PSocket(std::string base58_private_key);
	P2PSocket(std::string base58_private_key, DaemonConnectionImpl& parent_socket_manager);

	virtual ~P2PSocket();

	// Basically, accept and connect not only open "logical" socket between libp2pnet and p2pnetd, but
	// they open physical sockets, that are used to transfer data separately (I call them sub-sockets).
	P2PAssociation& accept();
	P2PAssociation& connect(std::string SH);

	void asyncAccept(std::function<void(P2PAssociation*)> accept_handler);
	void asyncConnect(std::string SH, std::function<void(P2PAssociation*)> connect_handler);

	void bind(std::string base58_private_key);	// Assigns private key to this socket.
};

} /* namespace p2pnet */

#endif /* P2PSOCKET_H_ */
