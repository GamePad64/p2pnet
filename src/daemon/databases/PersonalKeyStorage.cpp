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
#include "../../common/crypto/CurrentCipherSet.h"
#include <string>

namespace p2pnet {
namespace databases {

PersonalKeyStorage::PersonalKeyStorage() {}
PersonalKeyStorage::~PersonalKeyStorage() {}

void PersonalKeyStorage::regenerateKeys() {
	crypto::CurrentCipherSet cs;
	my_key_pair = cs.generateKeyPair();
	my_transport_hash = cs.computeHash(std::string(my_key_pair.key_public.begin(), my_key_pair.key_public.end()));
}

crypto::hash_t PersonalKeyStorage::getMyTransportHash() {
	return my_transport_hash;
}

crypto::key_public_t PersonalKeyStorage::getMyPublicKey() {
	return my_key_pair.key_public;
}

crypto::key_private_t PersonalKeyStorage::getMyPrivateKey() {
	return my_key_pair.key_private;
}

crypto::key_pair_t PersonalKeyStorage::getMyKeyPair() {
	return my_key_pair;
}

} /* namespace databases */
} /* namespace p2pnet */

