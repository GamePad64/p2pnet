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

#include "PublicKeyDSA.h"
#include "PrivateKeyDSA.h"
#include "Hash.h"
#include <botan/pubkey.h>

namespace p2pnet {
namespace crypto {

PublicKeyDSA::PublicKeyDSA() {}
PublicKeyDSA::PublicKeyDSA(const PrivateKeyDSA& privkey_dsa) {
	key_public = std::unique_ptr<Botan::ECDSA_PublicKey>(
			new Botan::ECDSA_PublicKey(*(privkey_dsa.key_private))
	);
}
PublicKeyDSA::PublicKeyDSA(const PublicKeyDSA& rvalue) {
	key_public = std::unique_ptr<Botan::ECDSA_PublicKey>(
			new Botan::ECDSA_PublicKey(*(rvalue.key_public))
	);
}
PublicKeyDSA::PublicKeyDSA(PublicKeyDSA&& rvalue) {
	std::swap(key_public, rvalue.key_public);
}
PublicKeyDSA::~PublicKeyDSA() {}
PublicKeyDSA& PublicKeyDSA::operator =(const PublicKeyDSA& rvalue) {
	key_public = std::unique_ptr<Botan::ECDSA_PublicKey>(
			new Botan::ECDSA_PublicKey(*(rvalue.key_public))
	);
	return *this;
}

PublicKeyDSA& PublicKeyDSA::operator =(PublicKeyDSA&& rvalue) {
	std::swap(key_public, rvalue.key_public);
	return *this;
}

bool PublicKeyDSA::isPresent() const {
	return bool(key_public);
}

std::string PublicKeyDSA::encrypt(std::string data) {
	Botan::PK_Encryptor_EME pk_encryptor(*key_public, "EME1(" + Hash::getAlgoName() + ")");
	Botan::AutoSeeded_RNG rng;

	std::vector<Botan::byte> encrypted_v = pk_encryptor.encrypt(std::vector<Botan::byte>(data.begin(), data.end()), rng);
	return std::string(encrypted_v.begin(), encrypted_v.end());
}

bool PublicKeyDSA::verify(std::string data, std::string signature) {
	Botan::PK_Verifier pk_verifier(*key_public, "EMSA1(" + Hash::getAlgoName() + ")");

	return pk_verifier.verify_message(
			std::vector<Botan::byte>(data.begin(), data.end()),
			std::vector<Botan::byte>(signature.begin(), signature.end())
			);
}

bool PublicKeyDSA::verifyRaw(std::string data, std::string signature) {
	Botan::PK_Verifier pk_verifier(*key_public, "Raw");

	return pk_verifier.verify_message(
			std::vector<Botan::byte>(data.begin(), data.end()),
			std::vector<Botan::byte>(signature.begin(), signature.end())
			);
}

void PublicKeyDSA::fromPEM(std::string pem) {
	std::vector<Botan::byte> pem_v(pem.begin(), pem.end());
	Botan::DataSource_Memory botan_source(pem_v);
	key_public = std::unique_ptr<Botan::ECDSA_PublicKey>(dynamic_cast<Botan::ECDSA_PublicKey*>(Botan::X509::load_key(botan_source)));
}

std::string PublicKeyDSA::toPEM() {
	return Botan::X509::PEM_encode(*key_public);
}

bool PublicKeyDSA::validate() {
	Botan::AutoSeeded_RNG rng;
	return key_public->check_key(rng, false);
}

void PublicKeyDSA::setAsBinaryVector(binary_vector_t serialized_vector) {
	Botan::DataSource_Memory botan_source(serialized_vector);
	Botan::ECDSA_PublicKey* pubkey = dynamic_cast<Botan::ECDSA_PublicKey*>(Botan::X509::load_key(botan_source));
	key_public = std::unique_ptr<Botan::ECDSA_PublicKey>(pubkey);
}

PublicKeyDSA::binary_vector_t PublicKeyDSA::toBinaryVector() const {
	std::vector<Botan::byte> pubkey_encoded_v = Botan::X509::BER_encode(*key_public);
	binary_vector_t pubkey_encoded_bv(pubkey_encoded_v.begin(), pubkey_encoded_v.end());
	return pubkey_encoded_bv;
}

} /* namespace crypto */
} /* namespace p2pnet */
