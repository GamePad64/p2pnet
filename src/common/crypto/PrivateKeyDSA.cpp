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

const Botan::ECDSA_PublicKey& PrivateKeyDSA::getPublicKey() {
	return *key_private;
}

const Botan::ECDSA_PrivateKey& PrivateKeyDSA::getPrivateKey() {
	return *key_private;
}

PrivateKeyDSA::PrivateKeyDSA() {}
PrivateKeyDSA::~PrivateKeyDSA() {}

void PrivateKeyDSA::generate(){
	key_private = std::make_shared<Botan::ECDSA_PrivateKey>(rng, Botan::EC_Group(curve));
}

std::string PrivateKeyDSA::decrypt(std::string enc_data) {
	Hash h;
	Botan::PK_Decryptor_EME pk_decryptor(getPrivateKey(), "EME1(" + h.getAlgoName() + ")");

	binary_vector_t decrypted_data_bv = pk_decryptor.decrypt(std::vector<Botan::byte>(enc_data.begin(), enc_data.end()));
	std::string decrypted_data_s(decrypted_data_bv.begin(), decrypted_data_bv.end());

	return decrypted_data_s;
}

std::string PrivateKeyDSA::sign(std::string data){
	Hash h;
	Botan::PK_Signer pk_signer(*key_private, "EMSA1(" + h.getAlgoName() + ")");

	std::vector<Botan::byte> message_v = pk_signer.sign_message(std::vector<Botan::byte>(data.begin(), data.end()), rng);
	std::string message_s(message_v.begin(), message_v.end());
	return message_s;
}

std::string PrivateKeyDSA::signRaw(std::string data){
	Hash h;
	Botan::PK_Signer pk_signer(*key_private, "Raw");

	std::vector<Botan::byte> message_v = pk_signer.sign_message(std::vector<Botan::byte>(data.begin(), data.end()), rng);
	std::string message_s(message_v.begin(), message_v.end());
	return message_s;
}

void PrivateKeyDSA::fromPEM(std::string pem) {
	std::vector<Botan::byte> pem_v(pem.begin(), pem.end());
	Botan::DataSource_Memory botan_source(pem_v);
	key_private = Botan::PKCS8::load_key(botan_source, rng);
}

std::string PrivateKeyDSA::toPEM() {
	return Botan::PKCS8::PEM_encode(getPrivateKey());
}

void PrivateKeyDSA::fromBinaryVector(binary_vector_t serialized_vector) {
	Botan::DataSource_Memory botan_source(serialized_vector);
	key_private = std::make_shared<ECDSA_PrivateKey>(Botan::PKCS8::load_key(botan_source, rng));
}

const PrivateKeyDSA::binary_vector_t PrivateKeyDSA::toBinaryVector() const {
	return Botan::PKCS8::BER_encode(*key_private);
}

} /* namespace crypto */
} /* namespace p2pnet */
