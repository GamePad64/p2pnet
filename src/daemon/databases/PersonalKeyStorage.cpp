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

PersonalKeyStorage::PersonalKeyStorage() {
	// TEMP: We shouldn't regenerate keys every execution, so some sort of caching is required.
	regenerateKeys();
}
PersonalKeyStorage::~PersonalKeyStorage() {}

void PersonalKeyStorage::regenerateKeys() {
	my_private_key.generate();
	my_transport_hash.compute(my_private_key.toBinaryString());

	std::clog << "[Crypto] Keys regenerated. New TH: " << my_transport_hash.toBase58() << std::endl;
}

crypto::Hash PersonalKeyStorage::getMyTransportHash() {
	return my_transport_hash;
}

crypto::PublicKeyDSA PersonalKeyStorage::getMyPublicKey() {
	return my_private_key;
}

crypto::PrivateKeyDSA PersonalKeyStorage::getMyPrivateKey() {
	return my_private_key;
}

} /* namespace databases */
} /* namespace p2pnet */

