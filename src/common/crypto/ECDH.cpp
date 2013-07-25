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

#include <botan/pubkey.h>
#include "ECDH.h"
#include "Hash.h"

namespace p2pnet {
namespace crypto {

ECDH::ECDH() {}

ECDH::ECDH(binary_vector_t serialized_vector) {
	setAsBinaryVector(serialized_vector);
}

ECDH::~ECDH() {
}

void ECDH::renewKey() {
	Botan::AutoSeeded_RNG rng;
	key_mutex.lock();
	key_private = std::make_shared<Botan::ECDH_PrivateKey>(rng, Botan::EC_Group(ecdh_curve));
	key_mutex.unlock();
}

ECDH ECDH::generateKey() {
	ECDH new_ecdh;
	new_ecdh.renewKey();
	return new_ecdh;
}

std::string ECDH::deriveSymmetricKey(size_t key_length, std::string other_pubkey, std::string session_param) const {
	key_mutex.lock();
	Botan::PK_Key_Agreement key_agreement(*key_private, "KDF2(" + Hash::getAlgoName() + ")");
	key_mutex.unlock();

	std::vector<Botan::byte> other_pubkey_v(other_pubkey.begin(), other_pubkey.end());

	return key_agreement.derive_key(key_length, other_pubkey_v, session_param).as_string();
}

std::string ECDH::derivePublicKey() const {
	key_mutex.lock();
	std::vector<Botan::byte> key_public_v = key_private->public_value();
	key_mutex.unlock();
	return std::string(key_public_v.begin(), key_public_v.end());
}

void ECDH::setAsBinaryVector(binary_vector_t serialized_vector) {
	Botan::DataSource_Memory botan_source(serialized_vector);
	Botan::AutoSeeded_RNG rng;
	Botan::ECDH_PrivateKey* privkey = dynamic_cast<Botan::ECDH_PrivateKey*>(Botan::PKCS8::load_key(botan_source, rng));
	key_mutex.lock();
	key_private = std::shared_ptr<Botan::ECDH_PrivateKey>(privkey);
	key_mutex.unlock();
}

const ECDH::binary_vector_t ECDH::toBinaryVector() const {
	key_mutex.lock();
	auto return_vector = Botan::PKCS8::BER_encode(*key_private);
	key_mutex.unlock();
	return return_vector;
}

} /* namespace crypto */
} /* namespace p2pnet */
