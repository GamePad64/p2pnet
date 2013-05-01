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

#include "PrivateKeyDSA.h"
#include "Hash.h"
#include <botan/pubkey.h>
#include <memory>

namespace p2pnet {
namespace crypto {

std::shared_ptr<Botan::ECDSA_PrivateKey> PrivateKeyDSA::getPrivateKeyPtrFromBinaryVector(binary_vector_t serialized_vector) {
	Botan::DataSource_Memory botan_source(serialized_vector);
	Botan::AutoSeeded_RNG rng;
	Botan::ECDSA_PrivateKey* privkey = dynamic_cast<Botan::ECDSA_PrivateKey*>(Botan::PKCS8::load_key(botan_source, rng));
	return std::shared_ptr<Botan::ECDSA_PrivateKey>(privkey);
}

const Botan::ECDSA_PrivateKey& PrivateKeyDSA::getPrivateKey() {
	return key_private;
}

PrivateKeyDSA::PrivateKeyDSA(binary_vector_t serialized_vector) :
		MathString<PrivateKeyDSA>::MathString(serialized_vector),
		key_private(*(getPrivateKeyPtrFromBinaryVector(serialized_vector))) {
}
PrivateKeyDSA::PrivateKeyDSA(Botan::ECDSA_PrivateKey& botan_key) :
		MathString<PrivateKeyDSA>::MathString(Botan::PKCS8::BER_encode(botan_key)),
		key_private(botan_key) {}
PrivateKeyDSA::~PrivateKeyDSA() {}

PrivateKeyDSA PrivateKeyDSA::generate(){
	Botan::AutoSeeded_RNG rng;

	Botan::ECDSA_PrivateKey botan_key(rng, Botan::EC_Group(curve));
	return PrivateKeyDSA(botan_key);
}

std::string PrivateKeyDSA::decrypt(std::string enc_data) {
	Botan::PK_Decryptor_EME pk_decryptor(getPrivateKey(), "EME1(" + Hash::getAlgoName() + ")");

	binary_vector_t decrypted_data_bv = pk_decryptor.decrypt(std::vector<Botan::byte>(enc_data.begin(), enc_data.end()));
	std::string decrypted_data_s(decrypted_data_bv.begin(), decrypted_data_bv.end());

	return decrypted_data_s;
}

std::string PrivateKeyDSA::sign(std::string data){
	Botan::PK_Signer pk_signer(key_private, "EMSA1(" + Hash::getAlgoName() + ")");
	Botan::AutoSeeded_RNG rng;

	std::vector<Botan::byte> message_v = pk_signer.sign_message(std::vector<Botan::byte>(data.begin(), data.end()), rng);
	std::string message_s(message_v.begin(), message_v.end());
	return message_s;
}

std::string PrivateKeyDSA::signRaw(std::string data){
	Botan::PK_Signer pk_signer(key_private, "Raw");
	Botan::AutoSeeded_RNG rng;

	std::vector<Botan::byte> message_v = pk_signer.sign_message(std::vector<Botan::byte>(data.begin(), data.end()), rng);
	std::string message_s(message_v.begin(), message_v.end());
	return message_s;
}

void PrivateKeyDSA::fromPEM(std::string pem) {
	Botan::AutoSeeded_RNG rng;

	std::vector<Botan::byte> pem_v(pem.begin(), pem.end());
	Botan::DataSource_Memory botan_source(pem_v);
	auto key_private_ptr = Botan::PKCS8::load_key(botan_source, rng);
	key_private = *dynamic_cast<Botan::ECDSA_PrivateKey*>(key_private_ptr);
	delete key_private_ptr;
}

std::string PrivateKeyDSA::toPEM() {
	return Botan::PKCS8::PEM_encode(getPrivateKey());
}

const PrivateKeyDSA::binary_vector_t PrivateKeyDSA::toBinaryVector() const {
	return Botan::PKCS8::BER_encode(key_private);
}

PublicKeyDSA PrivateKeyDSA::derivePublicKey() {
	return PublicKeyDSA(key_private);
}

} /* namespace crypto */
} /* namespace p2pnet */
