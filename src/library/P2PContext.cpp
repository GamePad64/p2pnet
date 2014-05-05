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
#include "../p2pnet.h"

#include "Impl.h"

namespace p2pnet {

P2PContext::P2PContext(ContextType context_type, P2PSocket* parent_socket) {
	impl = new Impl;
	impl->m_context_type = context_type;
	impl->m_parent_socket = parent_socket;
}

P2PContext::~P2PContext() {
	delete impl;
}

ContextType P2PContext::getContextType() const {
	return impl->m_context_type;
}
LIBP2PNET_DLL_EXPORTED ContextType p2p_getContextType(P2PContext* context) {return context->getContextType();};

uint32_t P2PContext::getContextID() const {
	return impl->context_id;
}
LIBP2PNET_DLL_EXPORTED uint32_t p2p_getContextID(P2PContext* context) {return context->getContextID();};

P2PSocket* P2PContext::getParentSocket() {
	return impl->m_parent_socket;
}
LIBP2PNET_DLL_EXPORTED P2PSocket* p2p_getParentSocket(P2PContext* context) {return context->getParentSocket();};

} /* namespace p2pnet */
