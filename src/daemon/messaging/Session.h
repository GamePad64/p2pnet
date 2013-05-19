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

#ifndef SESSION_H_
#define SESSION_H_

#include "../peer/TH.h"
#include "../../common/crypto/ECDH.h"
#include "../protobuf/NetDBEntry.pb.h"
#include <string>
#include <memory>

namespace p2pnet {
namespace messaging {

class Session {
	Session(peer::TH th);

	databases::NetDBEntry& m_netdb_entry;
	crypto::ECDH* m_ecdh_private_key;
public:
	virtual ~Session();

	typedef std::shared_ptr<Session> pointer;

	databases::NetDBEntry& getNetDBEntry();

	const crypto::ECDH& renewECDHPrivateKey();
	const crypto::ECDH& getECDHPrivateKey();
};

} /* namespace messaging */
} /* namespace p2pnet */
#endif /* SESSION_H_ */
