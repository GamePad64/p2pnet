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

#include <string>

namespace p2pnet {

const std::string PROGRAM_VERSION = "0.0.0-dev";

class Version {
	unsigned short major_version;
	unsigned short minor_version;
	unsigned short bugfix_version;

	std::string variant_version;
public:
	Version();
	Version(unsigned short major_version, unsigned short minor_version, unsigned short bugfix_version);
	Version(unsigned short major_version, unsigned short minor_version, unsigned short bugfix_version, std::string variant_version);
	virtual ~Version();

	void fromString(std::string version_s);
	std::string toString();
	Version(std::string version_s);

	void fromInteger(uint32_t version_int);
	uint32_t toInteger();
	Version(uint32_t version_int);

	// relational operators
	bool operator==(Version& lhs);
	bool operator<(Version& lhs);
	// equivalents
	bool operator!=(Version& lhs);
	bool operator>(Version& lhs);
	bool operator<=(Version& lhs);
	bool operator>=(Version& lhs);
};

Version getMyVersion();

} /* namespace p2pnet */
#endif /* VERSION_H_ */
