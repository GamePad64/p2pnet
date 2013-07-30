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

#ifndef REJECTEXCEPTION_H_
#define REJECTEXCEPTION_H_

#include <exception>
#include <string>

namespace p2pnet {
namespace messaging {

enum Reason {
	CUSTOM,
	CRC_MISMATCH,
	KEY_INVALID,
	PARSE_ERROR,
	ENCRYPTION_NEEDED
};

class RejectException {
	std::string m_comment;
	Reason m_reason;
	std::string m_component;
public:
	RejectException(Reason reason, std::string component);
	RejectException(std::string custom_reason, std::string component);
	RejectException(Reason reason, std::string comment, std::string component);

	std::string what() const;
};

} /* namespace messaging */
} /* namespace p2pnet */
#endif /* REJECTEXCEPTION_H_ */
