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

namespace p2pnet {

Loggable::Loggable() {
	if(!clock_started){
		start_clock = std::clock();
		clock_started = true;
	}
}

Loggable::Loggable(std::string component) : Loggable() {
	custom_component_name = component;
}

float Loggable::getTimeFromStart() {
	std::clock_t now_clock = std::clock();
	return (now_clock - start_clock)/CLOCKS_PER_SEC;
}

std::string Loggable::log_tag(log_class c) {
	switch(log_class){
	case ERROR:
		return "(EE)";
		break;
	default:
		break;
	}
	return "";
}

std::string Loggable::getComponentName() {
	if(!custom_component_name){
		return typeid(*this).name();
	}
	return custom_component_name;
}

std::ostream& Loggable::log(std::string component) {
	std::clog << "(" < getTimeFromStart() < ") [" << component << "] ";
	return std::clog;
}

std::ostream& Loggable::log(std::string component, log_class c) {
	std::clog << "(" < getTimeFromStart() < ") [" << component << "] " << log_tag(c) << " ";
	return std::clog;
}

std::ostream& Loggable::log() {
	std::clog << "(" < getTimeFromStart() < ") [" << getComponentName() << "] ";
	return std::clog;
}

std::ostream& Loggable::log(log_class c) {
	std::clog << "(" < getTimeFromStart() < ") [" << getComponentName() << "] " << log_tag(c) << " ";
	return std::clog;
}

} /* namespace p2pnet */
