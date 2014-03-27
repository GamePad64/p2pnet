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

namespace p2pnet {
namespace impl {

class P2PContextImpl {
public:
	enum ContextType {
		TYPE_SEQPACKET = 0,
		TYPE_STREAM = 1,
		TYPE_DATAGRAM = 2,
		TYPE_ANY = 255
	};

	P2PContextImpl(ContextType context_type, P2PSocketImpl* parent_socket);
	virtual ~P2PContextImpl();

	ContextType getContextType() const;

private:
	ContextType m_context_type;
	uint32_t m_id;
	P2PSocketImpl* m_parent_socket;
};

} /* namespace impl */
} /* namespace p2pnet */

#endif /* P2PCONTEXT_H_ */
