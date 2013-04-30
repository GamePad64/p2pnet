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
#include "Hash.h"
#include <botan/pubkey.h>

namespace p2pnet {
namespace crypto {

PublicKeyDSA::PublicKeyDSA(binary_vector_t serialized_vector) :
		MathString<PublicKeyDSA>::MathString(serialized_vector),
		key_public(*(getPublicKeyPtrFromBinaryVector(serialized_vector))) {}
PublicKeyDSA::PublicKeyDSA(Botan::ECDSA_PublicKey& botan_key) :
		MathString<PublicKeyDSA>::MathString([&](){	// Sort of type conversion crap. Uses lambdas.
	auto vec = Botan::X509::BER_encode(botan_key);
	return binary_vector_t(vec.begin(), vec.end());
}()),	key_public(botan_key) {}
PublicKeyDSA::~PublicKeyDSA() {}

std::shared_ptr<Botan::ECDSA_PublicKey> PublicKeyDSA::getPublicKeyPtrFromBinaryVector(binary_vector_t serialized_vector) {
	Botan::DataSource_Memory botan_source(serialized_vector);
	Botan::ECDSA_PublicKey* pubkey = dynamic_cast<Botan::ECDSA_PublicKey*>(Botan::X509::load_key(botan_source));
	return std::shared_ptr<Botan::ECDSA_PublicKey>(pubkey);
}

const Botan::ECDSA_PublicKey& PublicKeyDSA::getPublicKey() {
	return key_public;
}

std::string PublicKeyDSA::encrypt(std::string data) {
	Botan::PK_Encryptor_EME pk_encryptor(getPublicKey(), "EME1(" + Hash::getAlgoName() + ")");
	Botan::AutoSeeded_RNG rng;

	std::vector<Botan::byte> encrypted_v = pk_encryptor.encrypt(std::vector<Botan::byte>(data.begin(), data.end()), rng);
	return std::string(encrypted_v.begin(), encrypted_v.end());
}

bool PublicKeyDSA::verify(std::string data, std::string signature) {
	Botan::PK_Verifier pk_verifier(key_public, "EMSA1(" + Hash::getAlgoName() + ")");

	return pk_verifier.verify_message(
			std::vector<Botan::byte>(data.begin(), data.end()),
			std::vector<Botan::byte>(signature.begin(), signature.end())
			);
}

bool PublicKeyDSA::verifyRaw(std::string data, std::string signature) {
	Botan::PK_Verifier pk_verifier(key_public, "Raw");

	return pk_verifier.verify_message(
			std::vector<Botan::byte>(data.begin(), data.end()),
			std::vector<Botan::byte>(signature.begin(), signature.end())
			);
}

void PublicKeyDSA::fromPEM(std::string pem) {
	std::vector<Botan::byte> pem_v(pem.begin(), pem.end());
	Botan::DataSource_Memory botan_source(pem_v);
	auto key_public_ptr = Botan::X509::load_key(botan_source);
	key_public = *dynamic_cast<Botan::ECDSA_PublicKey*>(key_public_ptr);
	delete key_public_ptr;
}

std::string PublicKeyDSA::toPEM() {
	return Botan::X509::PEM_encode(getPublicKey());
}

bool PublicKeyDSA::validate() {
	Botan::AutoSeeded_RNG rng;
	return key_public.check_key(rng, false);
}

const PublicKeyDSA::binary_vector_t PublicKeyDSA::toBinaryVector() const {
	std::vector<Botan::byte> pubkey_encoded_v = Botan::X509::BER_encode(key_public);
	binary_vector_t pubkey_encoded_bv(pubkey_encoded_v.begin(), pubkey_encoded_v.end());
	return pubkey_encoded_bv;
}

} /* namespace crypto */
} /* namespace p2pnet */
