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
#ifndef P2PSTREAMCONTEXT_H_
#define P2PSTREAMCONTEXT_H_

#include "P2PContext.h"

namespace p2pnet {

class P2PStreamContext : public p2pnet::P2PContext {
public:
	P2PStreamContext();
	virtual ~P2PStreamContext();
};

} /* namespace p2pnet */

#endif /* P2PSTREAMCONTEXT_H_ */
