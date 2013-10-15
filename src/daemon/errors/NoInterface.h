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
#ifndef NOINTERFACE_H_
#define NOINTERFACE_H_

#include <string>

namespace p2pnet {
namespace errors {

class NoInterface {
public:
	NoInterface();
	virtual ~NoInterface();

	std::string what();
};

} /* namespace errors */
} /* namespace p2pnet */

#endif /* NOINTERFACE_H_ */
