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

#ifndef SESSIONMAP_H_
#define SESSIONMAP_H_

#include "../peer/TH.h"
#include "Session.h"

#include <map>
#include <boost/unordered_map.hpp>
#include <cpp-btree/safe_btree_map.h>

namespace p2pnet {
namespace messaging {

#if (STDMAPSESSION)
typedef std::map<std::vector<std::string, Session::pointer> SessionMap;
#elif (UNORDEREDMAPSESSION)
typedef boost::unordered_map<std::string, Session::pointer> SessionMap;
#elif (BMAPSESSION)
typedef btree::safe_btree_map<std::string, Session::pointer> SessionMap;
#else
typedef std::map<std::string, Session::pointer> SessionMap;
#endif

} /* namespace messaging */
} /* namespace p2pnet */
#endif /* SESSIONMAP_H_ */
