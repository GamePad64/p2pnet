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

PublicKeyDSA::PublicKeyDSA() {}
PublicKeyDSA::~PublicKeyDSA() {}

const Botan::ECDSA_PublicKey& PublicKeyDSA::getPublicKey() {
	return *key_public;
}

std::string PublicKeyDSA::encrypt(std::string data) {
	Hash h;
	Botan::PK_Encryptor_EME pk_encryptor(getPublicKey(), "EME1(" + h.getAlgoName() + ")");

	std::vector<Botan::byte> encrypted_v = pk_encryptor.encrypt(std::vector<Botan::byte>(data.begin(), data.end()), rng);
	return std::string(encrypted_v.begin(), encrypted_v.end());
}

bool PublicKeyDSA::verify(std::string data, std::string signature) {
	Hash h;
	Botan::PK_Verifier pk_verifier(*key_public, "EMSA1(" + h.getAlgoName() + ")");

	return pk_verifier.verify_message(
			std::vector<Botan::byte>(data.begin(), data.end()),
			std::vector<Botan::byte>(signature.begin(), signature.end())
			);
}

bool PublicKeyDSA::verifyRaw(std::string data, std::string signature) {
	Hash h;
	Botan::PK_Verifier pk_verifier(*key_public, "Raw");

	return pk_verifier.verify_message(
			std::vector<Botan::byte>(data.begin(), data.end()),
			std::vector<Botan::byte>(signature.begin(), signature.end())
			);
}

void PublicKeyDSA::fromPEM(std::string pem) {
	key_public = std::make_shared<Botan::ECDSA_PublicKey>(Botan::X509::load_key(std::vector<Botan::byte>(pem.begin(), pem.end())));
}

std::string PublicKeyDSA::toPEM() {
	return Botan::X509::PEM_encode(getPublicKey());
}

void PublicKeyDSA::fromBinaryVector(binary_vector_t serialized_vector) {
	Botan::DataSource_Memory botan_source(serialized_vector);
	key_public = std::make_shared<Botan::ECDSA_PublicKey>(Botan::X509::load_key(botan_source));
}

bool PublicKeyDSA::validate() {
	return key_public->check_key(rng, false);
}

const PublicKeyDSA::binary_vector_t PublicKeyDSA::toBinaryVector() const {
	std::vector<Botan::byte> pubkey_encoded_v = Botan::X509::BER_encode(*key_public);
	binary_vector_t pubkey_encoded_bv(pubkey_encoded_v.begin(), pubkey_encoded_v.end());
	return pubkey_encoded_bv;
}

} /* namespace crypto */
} /* namespace p2pnet */
