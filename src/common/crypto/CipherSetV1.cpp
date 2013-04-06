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

#include "CipherSetV1.h"
#include "../../common/crypto/Hash.h"
#include <botan/keccak.h>
#include <botan/rsa.h>

namespace p2pnet {
namespace crypto {

CipherSetV1::CipherSetV1() {}
CipherSetV1::~CipherSetV1() {}

Hash CipherSetV1::computeHash(const std::string& data) {
	Botan::Keccak_1600 hasher(KECCAK_LENGTH);
	Hash hash;
	hash.fromBinaryVector(hasher.process(data));
	return hash;
}

bool CipherSetV1::checkHash(const std::string& data, const Hash& hash) {
	return (computeHash(data).toBinaryVector() == hash.toBinaryVector());
}

key_pair_t CipherSetV1::generateKeyPair() {
	Botan::RSA_PrivateKey key(rng, RSA_LENGTH);

	key_pair_t keys;
	keys.key_public = Botan::X509::BER_encode(key);
	keys.key_private = Botan::PKCS8::BER_encode(key);
	return keys;
}

bool CipherSetV1::checkKeyPair(key_pair_t key_pair) {
	if(key_pair.key_private.empty() || key_pair.key_public.empty()){
		return false;
	}
	Botan::DataSource_Memory botan_source_private(key_pair.key_private);
	Botan::RSA_PrivateKey* key_private_botan = dynamic_cast<Botan::RSA_PrivateKey*>(Botan::PKCS8::load_key(botan_source_private, rng));

	Botan::DataSource_Memory botan_source_public(key_pair.key_public);
	Botan::RSA_PublicKey* key_public_botan = dynamic_cast<Botan::RSA_PublicKey*>(Botan::X509::load_key(botan_source_public));

	if(Botan::X509::BER_encode(*key_private_botan) != Botan::X509::BER_encode(*key_public_botan)){
		return false;
	}

	return key_private_botan->check_key(rng, false);
}

std::string CipherSetV1::privKeyToPEM(key_private_t key_private) {
	Botan::DataSource_Memory botan_source(key_private);
	Botan::RSA_PrivateKey* key_private_botan = dynamic_cast<Botan::RSA_PrivateKey*>(Botan::PKCS8::load_key(botan_source, rng));
	return Botan::PKCS8::PEM_encode(*key_private_botan);
}

std::string CipherSetV1::pubKeyToPEM(key_public_t key_public) {
	Botan::DataSource_Memory botan_source(key_public);
	Botan::RSA_PublicKey* key_public_botan = dynamic_cast<Botan::RSA_PublicKey*>(Botan::X509::load_key(botan_source));
	return Botan::X509::PEM_encode(*key_public_botan);
}

/*key_private_t CipherSetV1::PEMToPrivKey(std::string key_pem) {
}

key_public_t CipherSetV1::PEMToPubKey(std::string key_pem) {
}*/

} /* namespace crypto */
} /* namespace p2pnet */
