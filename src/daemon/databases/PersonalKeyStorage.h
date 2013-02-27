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

#include "../../common/crypto/CurrentCipherSet.h"
#include "../../common/crypto/CryptoTypes.h"

namespace p2pnet {
namespace databases {

class PersonalKeyStorage {
	crypto::key_pair_t my_keys;
	crypto::hash_t my_transport_hash;

protected:
	PersonalKeyStorage();
	virtual ~PersonalKeyStorage();

public:
	static PersonalKeyStorage* getInstance(){
		static PersonalKeyStorage* instance;
		if(!instance){
			instance = new PersonalKeyStorage();
		}
		return instance;
	}

	crypto::hash_t getMyTransportHash() {return my_transport_hash;};

	crypto::key_public_t getMyPublicKey();
	crypto::key_private_t getMyPrivateKey();
	crypto::key_pair_t getMyKeyPair();
};

} /* namespace databases */
} /* namespace p2pnet */
#endif /* PERSONALKEYSTORAGE_H_ */
