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
#include "../../common/crypto/PrivateKeyDSA.h"

#include <memory>

namespace p2pnet {

class P2PContext::Impl {
public:
	ContextType m_context_type;
	uint32_t m_id;
	P2PSocket* m_parent_socket;
};

class P2PSocket::Impl {
public:
	P2PNode* m_parent_node;
	P2PDaemon* m_parent_daemon_ptr;

	uint32_t node_id;
	uint32_t socket_id;
	std::map<uint32_t, P2PContext*> m_contexts;

	std::string m_remote_sh;
};

class P2PNode::Impl {
public:
	std::shared_ptr<P2PDaemon> m_parent_daemon_shared;
	P2PDaemon* m_parent_daemon_ptr;

	uint32_t node_id;
	crypto::PrivateKeyDSA m_bound_private_key;
};

} /* namespace p2pnet */

#endif /* IMPL_H_ */
