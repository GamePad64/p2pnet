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

#include "PersonalKeyStorage.h"
#include "../../common/crypto/Hash.h"
#include <string>
#include <iostream>

namespace p2pnet {
namespace databases {

PersonalKeyStorage::PersonalKeyStorage() : timer(timer_service) {
	// TEMP: We shouldn't regenerate keys every execution, so some sort of caching is required.
	regenerateKeys();
}
PersonalKeyStorage::~PersonalKeyStorage() {
	delete my_private_key;
	delete my_transport_hash;
}

void PersonalKeyStorage::regenerateKeys() {
	// Generating all this. This should be asynchronous.
	auto my_new_private_key = new crypto::PrivateKeyDSA(crypto::PrivateKeyDSA::generateKey());
	auto my_new_transport_hash = new crypto::Hash(
			crypto::Hash::compute(my_new_private_key->derivePublicKey().toBinaryString()));

	// LOCK! Moving faster now!
	key_lock.lock();
	// Synchronous zone. Now we freeze all the system.
	//delete my_private_key;
	//delete my_transport_hash;
	my_private_key = my_new_private_key;	// We will not delete my_new_private_key after =, it's ok.
	my_transport_hash = my_new_transport_hash;
	// UNLOCK! It's safe now.
	key_lock.unlock();

	std::clog << "[Crypto] Keys regenerated. New TH: " << my_transport_hash->toBase58() << std::endl;
}

crypto::Hash PersonalKeyStorage::getMyTransportHash() {
	key_lock.lock();
	auto ret = crypto::Hash(*my_transport_hash);
	key_lock.unlock();
	return ret;
}

crypto::PublicKeyDSA PersonalKeyStorage::getMyPublicKey() {
	key_lock.lock();
	auto ret = my_private_key->derivePublicKey();
	key_lock.unlock();
	return ret;
}

crypto::PrivateKeyDSA PersonalKeyStorage::getMyPrivateKey() {
	key_lock.lock();
	auto ret = crypto::PrivateKeyDSA(*my_private_key);
	key_lock.unlock();
	return ret;
}

} /* namespace databases */
} /* namespace p2pnet */

