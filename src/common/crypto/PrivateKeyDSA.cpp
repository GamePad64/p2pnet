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
#include "PublicKeyDSA.h"
#include "Hash.h"
#include <botan/pubkey.h>
#include <memory>

namespace p2pnet {
namespace crypto {

PrivateKeyDSA::PrivateKeyDSA() {
	key_private = nullptr;
}
PrivateKeyDSA::PrivateKeyDSA(const PrivateKeyDSA& rvalue) {
	key_private = std::unique_ptr<Botan::ECDSA_PrivateKey>(
			new Botan::ECDSA_PrivateKey(*(rvalue.key_private))
	);
}

PrivateKeyDSA::PrivateKeyDSA(PrivateKeyDSA&& rvalue) {
	std::swap(key_private, rvalue.key_private);
}
PrivateKeyDSA::~PrivateKeyDSA() {}

PrivateKeyDSA& PrivateKeyDSA::operator =(const PrivateKeyDSA& rvalue) {
	auto key_private_ptr = std::unique_ptr<Botan::ECDSA_PrivateKey>(new Botan::ECDSA_PrivateKey(*(rvalue.key_private)));
	std::swap(key_private, key_private_ptr);
	return *this;
}
PrivateKeyDSA& PrivateKeyDSA::operator =(PrivateKeyDSA&& rvalue) {
	std::swap(key_private, rvalue.key_private);
	return *this;
}

PrivateKeyDSA PrivateKeyDSA::generateNewKey(){
	PrivateKeyDSA privkey;
	privkey.generateKey();
	return privkey;
}

void PrivateKeyDSA::generateKey() {
	Botan::AutoSeeded_RNG rng;
	key_private = std::unique_ptr<Botan::ECDSA_PrivateKey>(
			new Botan::ECDSA_PrivateKey(rng, Botan::EC_Group(dsa_curve))
	);
}

std::string PrivateKeyDSA::decrypt(std::string enc_data) {
	Botan::PK_Decryptor_EME pk_decryptor(*key_private, "EME1(" + Hash::getAlgoName() + ")");

	binary_vector_t decrypted_data_bv = pk_decryptor.decrypt(std::vector<Botan::byte>(enc_data.begin(), enc_data.end()));
	std::string decrypted_data_s(decrypted_data_bv.begin(), decrypted_data_bv.end());

	return decrypted_data_s;
}

std::string PrivateKeyDSA::sign(std::string data) const{
	Botan::PK_Signer pk_signer(*key_private, "EMSA1(" + Hash::getAlgoName() + ")");
	Botan::AutoSeeded_RNG rng;

	std::vector<Botan::byte> message_v = pk_signer.sign_message(std::vector<Botan::byte>(data.begin(), data.end()), rng);
	std::string message_s(message_v.begin(), message_v.end());
	return message_s;
}

std::string PrivateKeyDSA::signRaw(std::string data) const{
	Botan::PK_Signer pk_signer(*key_private, "Raw");
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
	key_private = std::unique_ptr<Botan::ECDSA_PrivateKey>(dynamic_cast<Botan::ECDSA_PrivateKey*>(key_private_ptr));
}

std::string PrivateKeyDSA::toPEM() {
	return Botan::PKCS8::PEM_encode(*key_private);
}

void PrivateKeyDSA::fromBER(binary_vector_t ber_encoded) {
	Botan::DataSource_Memory botan_source(ber_encoded);
	Botan::AutoSeeded_RNG rng;
	Botan::ECDSA_PrivateKey* privkey = dynamic_cast<Botan::ECDSA_PrivateKey*>(Botan::PKCS8::load_key(botan_source, rng));
	key_private = std::unique_ptr<Botan::ECDSA_PrivateKey>(privkey);
}

PrivateKeyDSA::binary_vector_t PrivateKeyDSA::toBER() {
	return Botan::PKCS8::BER_encode(*key_private);
}

void PrivateKeyDSA::setAsBinaryVector(binary_vector_t serialized_vector) {
	Botan::AutoSeeded_RNG rng;
	Botan::BigInt bigint;
	bigint.binary_decode(serialized_vector);
	key_private = std::unique_ptr<Botan::ECDSA_PrivateKey>(new Botan::ECDSA_PrivateKey(rng, Botan::EC_Group(dsa_curve), bigint));
}

PrivateKeyDSA::binary_vector_t PrivateKeyDSA::toBinaryVector() const {
	auto private_value = key_private->private_value();

	PrivateKeyDSA::binary_vector_t vectorized_val(private_value.encoded_size()), result_vector(dsa_byte_length);

	private_value.binary_encode(vectorized_val.data());
	std::copy(vectorized_val.rbegin(), vectorized_val.rend(), result_vector.rbegin());

	return result_vector;
}

PublicKeyDSA PrivateKeyDSA::derivePublicKey() const{
	return PublicKeyDSA(*this);
}

} /* namespace crypto */
} /* namespace p2pnet */
