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
#ifndef LOGGABLE_H_
#define LOGGABLE_H_

#include <iostream>
#include <chrono>

std::string demangle(const char* name);

namespace p2pnet {

class Loggable {
public:
	enum log_class {
		NONE,
		ERROR
	};
private:
	static bool clock_started;
	static std::chrono::steady_clock::time_point start_clock;
	static float getTimeFromStart();

	static std::string log_tag(log_class c);
public:
	Loggable();
	virtual ~Loggable();

	std::string getComponentName();	// Basic implementation uses RTTI!

	static std::ostream& log(std::string component, log_class c = NONE);
	std::ostream& log(log_class c = NONE);
};

} /* namespace p2pnet */

#endif /* LOGGABLE_H_ */
