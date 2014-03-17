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
#ifndef P2PCONTEXT_H_
#define P2PCONTEXT_H_

#include "daemon_comm/ClientDataSocket.h"

namespace p2pnet {

class P2PContext {
public:
	enum ContextType {
		SEQUENCE = 0,
		STREAM = 1,
		DATAGRAM = 2
	};

	P2PContext(ContextType context_type, std::weak_ptr<P2PAssociation> parent_association);
	virtual ~P2PContext();

	ContextType getContextType() const;

private:
	ContextType m_context_type;
	std::weak_ptr<P2PAssociation> m_parent_association;

	impl::ClientDataSocket* m_data_socket;
};

} /* namespace p2pnet */

#endif /* P2PCONTEXT_H_ */
