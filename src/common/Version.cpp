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

#include "Version.h"
#include <sstream>

namespace p2pnet {

Version::Version() {
	this->major_version = 0;
	this->minor_version = 0;
	this->bugfix_version = 0;
}
Version::Version(unsigned short major_version, unsigned short minor_version, unsigned short bugfix_version) {
	this->major_version = major_version;
	this->minor_version = minor_version;
	this->bugfix_version = bugfix_version;
}
Version::Version(unsigned short major_version, unsigned short minor_version, unsigned short bugfix_version, std::string variant_version) {
	this->major_version = major_version;
	this->minor_version = minor_version;
	this->bugfix_version = bugfix_version;
	this->variant_version = variant_version;
}

Version::~Version() {}

void Version::fromString(std::string version_s) {
	char temp_ch;

	std::stringstream version_ss(version_s);
	version_ss >> major_version >> temp_ch >> minor_version >> temp_ch >> bugfix_version >> temp_ch >> variant_version;
}

std::string Version::toString() {
	std::stringstream version_ss;
	version_ss << major_version << "." << minor_version << "." << bugfix_version;
	if(!variant_version.empty()){
		version_ss << "-" << variant_version;
	}
	return version_ss.str();
}

Version::Version(std::string version_s){
	fromString(version_s);
}

bool Version::operator==(Version& lhs) {
	return (this->major_version == lhs.major_version && this->minor_version == lhs.minor_version && this->bugfix_version == lhs.bugfix_version);
}

bool Version::operator<(Version& lhs) {
	return ((this->major_version < lhs.major_version) ||
			(this->major_version == lhs.major_version && this->minor_version < lhs.minor_version) ||
			(this->major_version == lhs.major_version && this->minor_version == lhs.minor_version && this->bugfix_version < lhs.bugfix_version));
}

bool Version::operator!=(Version& lhs) {
	return !(*this == lhs);
}

bool Version::operator>(Version& lhs) {
	return lhs < *this;
}

bool Version::operator<=(Version& lhs) {
	return !(*this < lhs);
}

bool Version::operator>=(Version& lhs) {
	return !(lhs < *this);
}

Version getMyVersion(){
	return Version(PROGRAM_VERSION);
}

} /* namespace p2pnet */
