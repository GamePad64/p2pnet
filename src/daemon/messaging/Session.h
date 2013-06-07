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
#include "MessageGenerator.h"
#include <string>
#include <memory>

namespace p2pnet {
namespace messaging {

class Session {
	databases::NetDBEntry& m_netdb_entry;

	peer::TH m_th;
	crypto::ECDH* m_ecdh_private_key;

	// Agreement message reception mark. It may be fake (MitM attack, for example).
	bool agreement_received;
	std::string agreement_ecdh_public;	// This public key's signature is not checked!
	std::string agreement_signature;

	MessageGenerator m_generator;
public:
	Session(peer::TH th);
	virtual ~Session();

	typedef std::shared_ptr<Session> pointer;

	// Accessors
	databases::NetDBEntry& getNetDBEntry();

	bool hasECDHPrivateKey() const;
	const crypto::ECDH& renewECDHPrivateKey();
	const crypto::ECDH& getECDHPrivateKey();

	// Senders
	void sendKeyExchangeMessage();
	void sendAgreementMessage();
	void sendConnectionMessage();
};

} /* namespace messaging */
} /* namespace p2pnet */
#endif /* SESSION_H_ */
