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

#ifndef VERSION_H_
#define VERSION_H_

#include "../version.h"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

namespace p2pnet {

constexpr uint32_t getMyVersion(){
	return P2PNET_VERSION;
}

std::string makeVersionString(uint32_t major, uint32_t minor, uint32_t build, std::string channel){
	std::ostringstream os;
	os << major << '.' << std::setfill('0') << std::setw(2) << minor << '.' << std::setw(4) << build << '-' << channel;
	return os.str();
}

std::string getMyVersionString(){
	constexpr uint32_t major = getMyVersion()/1000000;
	constexpr uint32_t minor = (getMyVersion()-major*1000000)/10000;
	constexpr uint32_t build = getMyVersion()-major*1000000-minor*1000;

	return makeVersionString(major, minor, build, P2PNET_CHANNEL);
}

} /* namespace p2pnet */
#endif /* VERSION_H_ */
