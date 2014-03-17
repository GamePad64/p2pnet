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
#ifndef P2PASSOCIATION_H_
#define P2PASSOCIATION_H_

#include "P2PStreamContext.h"
#include "P2PDatagramContext.h"

#include <vector>

namespace p2pnet {

class P2PAssociation {
	std::vector<std::shared_ptr<P2PContext>> m_contexts;

	crypto::PrivateKeyDSA m_bound_key;
	crypto::PrivateKeyDSA m_local_assoc_key;

	std::string m_remote_sh;
public:
	P2PAssociation();
	virtual ~P2PAssociation();

	std::shared_ptr<P2PSequenceContext> createSequenceContext();
	std::shared_ptr<P2PStreamContext> createStreamContext();
	std::shared_ptr<P2PDatagramContext> createDatagramContext();

	std::shared_ptr<P2PSequenceContext> acceptSequenceContext();
	std::shared_ptr<P2PStreamContext> acceptStreamContext();
	std::shared_ptr<P2PDatagramContext> acceptDatagramContext();

	std::shared_ptr<P2PContext> acceptAnyContext();

	void closeContext(std::shared_ptr<P2PContext> context_ptr);
	void closeContext(int context_number);	// "0" is hardcore here.

	// Everything else. Maybe, group association?
};

} /* namespace p2pnet */

#endif /* P2PASSOCIATION_H_ */
