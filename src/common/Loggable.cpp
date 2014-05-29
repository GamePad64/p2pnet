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

#include "Loggable.h"
#include <typeinfo>

namespace p2pnet {

std::chrono::steady_clock::time_point Loggable::start_clock;
bool Loggable::clock_started = false;

Loggable::Loggable() {
	if(!clock_started){
		start_clock = std::chrono::steady_clock::now();
		clock_started = true;
	}
}

Loggable::~Loggable() {}

float Loggable::getTimeFromStart() {
	std::chrono::steady_clock::time_point now_clock = std::chrono::steady_clock::now();
	auto time_span = std::chrono::duration_cast<std::chrono::duration<double>>(now_clock - start_clock);
	return time_span.count();
}

std::string Loggable::log_tag(log_class c) {
	switch(c){
	case ERROR:
		return "(EE) ";
		break;
	default:
		break;
	}
	return "";
}

std::string Loggable::getComponentName() {
	auto demangled_name = demangle(typeid(*this).name());
	if(demangled_name.compare(0, 8, "p2pnet::") == 0){
		return demangled_name.substr(8);
	}
	return demangled_name;
}

std::ostream& Loggable::log(std::string component, log_class c) {
	std::clog << "(" << getTimeFromStart() << ") [" << component << "] " << log_tag(c);
	return std::clog;
}

std::ostream& Loggable::log(log_class c) {
	return log(getComponentName(), c);
}

} /* namespace p2pnet */
