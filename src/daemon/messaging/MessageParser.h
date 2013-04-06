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

#ifndef MESSAGEPARSER_H_
#define MESSAGEPARSER_H_

#include "../protobuf/Protocol.pb.h"

namespace p2pnet {
namespace messaging {

class MessageParser {
	protocol::p2pMessageHeader generateMessageHeader() const;
	protocol::p2pMessageHeader generateMessageHeader(const crypto::Hash dest) const;
//	protocol::p2pMessageHeader generateMessageHeader(const crypto::Hash src, const crypto::Hash dest) const;
public:
	MessageParser();
	virtual ~MessageParser();

	protocol::p2pMessage generateAgreementMessage();
};

} /* namespace messaging */
} /* namespace p2pnet */
#endif /* MESSAGEPARSER_H_ */
