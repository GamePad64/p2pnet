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
#pragma once

#include <memory>

namespace p2pnet {
namespace transport {

class InterfaceEndpoint {
	const std::string prefix;
public:
	InterfaceEndpoint(const std::string& prefix) : prefix(prefix){}
	virtual ~InterfaceEndpoint(){}

	virtual std::shared_ptr<InterfaceEndpoint> clone() = 0;
	std::string getPrefix() const {return prefix;}

	/*
	 * Readable strings.
	 */
	virtual std::string toString() const = 0;
	virtual void fromString(const std::string& string) = 0;
};

} /* namespace net */
} /* namespace p2pnet */
