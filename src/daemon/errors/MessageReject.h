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

//BIG SHIT! This exception is made using streams, strings, and other memory-allocating stuff.
//So, now we know, that this is out-of-guideliney stuff.
#ifndef MESSAGEREJECT_H_
#define MESSAGEREJECT_H_

namespace p2pnet {
namespace errors {

class MessageReject {
public:
	enum Reason {
		CUSTOM,
		CRC_MISMATCH,
		KEY_INVALID,
		PARSE_ERROR,
		ENCRYPTION_NEEDED
	};
private:
	std::string m_comment;
	Reason m_reason;
public:
	MessageReject(Reason reason);
	MessageReject(std::string custom_reason);
	MessageReject(Reason reason, std::string comment);

	std::string what() const;
};

} /* namespace errors */
} /* namespace p2pnet */

#endif /* MESSAGEREJECT_H_ */
