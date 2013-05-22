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

#include "RejectException.h"
#include <iostream>
#include <sstream>

namespace p2pnet {
namespace messaging {

RejectException::RejectException(Reason reason) : m_reason(reason) {}

RejectException::RejectException(std::string custom_reason) : m_comment(custom_reason), m_reason(Reason::CUSTOM) {
}

RejectException::RejectException(Reason reason, std::string comment) : m_comment(comment), m_reason(reason) {
}

std::string p2pnet::messaging::RejectException::what() const {
	std::ostringstream what;
	what << "[MessageSocket] (EE) Rejected";
	if(m_reason != CUSTOM){
		what << ". ";
		switch(m_reason){
		case CRC_MISMATCH:
			what << "CRC mismatch";
			break;
		case KEY_INVALID:
			what << "Public key failure";
			break;
		default:;
		}
	}

	if(!m_comment.empty()){
		what << ": " << m_comment;
	}

	what << ".";
	return what.str();
}

} /* namespace messaging */
} /* namespace p2pnet */
