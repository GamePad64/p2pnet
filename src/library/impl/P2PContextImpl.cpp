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
#include "P2PContextImpl.h"

namespace p2pnet {

P2PContext::P2PContext(ContextType context_type, std::weak_ptr<P2PAssociationImpl> parent_association) :
		m_context_type(context_type),
		m_parent_association(parent_association) {
}

P2PContext::~P2PContext() {
	// TODO Auto-generated destructor stub
}

P2PContext::ContextType P2PContext::getContextType() const {
	return m_context_type;
}

} /* namespace p2pnet */
