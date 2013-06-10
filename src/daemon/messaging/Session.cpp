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

#include "Session.h"
#include "../databases/NetDBStorage.h"

namespace p2pnet {
namespace messaging {

Session::Session(peer::TH th) : m_th(th), m_netdb_entry(databases::NetDBStorage::getInstance()->getEntry(th)),
		m_ecdh_private_key(NULL) {
}

Session::~Session() {
	delete m_ecdh_private_key;
}

databases::NetDBEntry& Session::getNetDBEntry() {
	return m_netdb_entry;
}

bool Session::hasECDHPrivateKey() const {
	return (bool)m_ecdh_private_key;
}

const crypto::ECDH& Session::renewECDHPrivateKey() {
	delete m_ecdh_private_key;
	m_ecdh_private_key = new crypto::ECDH(crypto::ECDH::generate());
	return *(m_ecdh_private_key);
}

const crypto::ECDH& Session::getECDHPrivateKey() {
	if(m_ecdh_private_key){
		return *m_ecdh_private_key;
	}
	return renewECDHPrivateKey();
}

void Session::sendKeyExchangeMessage() {

}

void Session::sendConnectionMessage() {
	protocol::p2pMessage_Payload payload = m_generator.generateConnectionPayload(getECDHPrivateKey().derivePublicKey());

	auto pks = databases::PersonalKeyStorage::getInstance();
	auto message = m_generator.generateMessage(pks->getMyTransportHash(), m_th, payload);
}

} /* namespace messaging */
} /* namespace p2pnet */
