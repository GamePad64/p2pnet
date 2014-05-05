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
#ifndef IMPL_H_
#define IMPL_H_

#include "../p2pnet.h"
#include "../common/crypto/PrivateKeyDSA.h"

#include <memory>
#include <future>

namespace p2pnet {

class DaemonIO {
public:
	P2PDaemon* parent_daemon;
};

class P2PContext::Impl : public DaemonIO {
public:
	P2PSocket* m_parent_socket;

	ContextType m_context_type;

	uint32_t node_id;
	uint32_t socket_id;
	uint32_t context_id;
};

class P2PSocket::Impl : public DaemonIO {
public:
	P2PNode* m_parent_node;

	std::map<uint32_t, P2PContext*> m_contexts;
	std::string m_remote_sh;

	uint32_t node_id;
	uint32_t socket_id;
};

class P2PNode::Impl : public DaemonIO {
public:
	std::shared_ptr<P2PDaemon> m_parent_daemon_shared;

	crypto::PrivateKeyDSA m_bound_private_key;

	uint32_t node_id;
};

} /* namespace p2pnet */

#endif /* IMPL_H_ */
