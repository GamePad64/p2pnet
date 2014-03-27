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
#ifndef P2PSOCKETIMPL_H_
#define P2PSOCKETIMPL_H_

#include "../common/crypto/PrivateKeyDSA.h"
#include "P2PSocketImpl.h"
#include "P2PDaemonConnectionImpl.h"

namespace p2pnet {
namespace impl {

class P2PSocketImpl {
	P2PDaemonConnectionImpl* m_parent_socket_manager;
	std::shared_ptr<api::APIClient> m_api_client;

	uint32_t socket_id;
	std::map<uint32_t, P2PContextImpl*> m_contexts;

	crypto::PrivateKeyDSA m_bound_private_key;
	crypto::PrivateKeyDSA m_connected_private_key;
	std::string m_remote_sh;
public:
	P2PSocketImpl();
	P2PSocketImpl(P2PDaemonConnectionImpl* parent_socket_manager);
	P2PSocketImpl(std::string base58_private_key);
	P2PSocketImpl(std::string base58_private_key, P2PDaemonConnectionImpl* parent_socket_manager);

	virtual ~P2PSocketImpl();

	// Connection-control methods
	void bind(std::string base58_private_key);	// Assigns private key to this socket.

	P2PSocketImpl* accept();
	void connect(std::string SH);
	void disconnect();

	void listen();

	// Context-control methods
	P2PContextImpl* createContext(P2PContextImpl::ContextType type);
	P2PContextImpl* acceptContext(P2PContextImpl::ContextType type);

	void closeContext(P2PContextImpl* context_ptr);
	void closeContext(uint32_t context_number);	// "0" is really a synonym to disconnect().
};

} /* namespace impl */
} /* namespace p2pnet */

#endif /* P2PSOCKETIMPL_H_ */
