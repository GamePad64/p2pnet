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

#ifndef PERSONALKEYSTORAGE_H_
#define PERSONALKEYSTORAGE_H_

#include "../../common/crypto/PrivateKeyDSA.h"
#include "../peer/TH.h"

namespace p2pnet {
namespace databases {

class PersonalKeyStorage {
	crypto::PrivateKeyDSA* my_private_key;
	peer::TH* my_transport_hash;

protected:
	PersonalKeyStorage();

public:
	static PersonalKeyStorage* getInstance(){
		static PersonalKeyStorage* instance;
		if(!instance){
			instance = new PersonalKeyStorage();
		}
		return instance;
	}
	virtual ~PersonalKeyStorage();

	void regenerateKeys();

	/**
	 * This function returns own transport hash from cache. If there is no such hash, then regenerate keys and hashes.
	 * @return My transport hash
	 */
	peer::TH getMyTransportHash();

	crypto::PublicKeyDSA getMyPublicKey();
	crypto::PrivateKeyDSA getMyPrivateKey();
};

} /* namespace databases */
} /* namespace p2pnet */
#endif /* PERSONALKEYSTORAGE_H_ */
