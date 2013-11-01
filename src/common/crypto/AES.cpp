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
#include "AES.h"

namespace p2pnet {
namespace crypto {

AES::AES() {}
AES::~AES() {}

bool AES::isPresent() const {
	return key.length() > 0 && iv.length() > 0;
}

std::string AES::encrypt(std::string data) const {
	Botan::AutoSeeded_RNG rng;
	Botan::Pipe pipe(get_cipher("AES-128/CBC", key, iv, Botan::ENCRYPTION));
	pipe.process_msg(data);

	return pipe.read_all_as_string(0);
}

std::string AES::decrypt(std::string data) const {
	Botan::AutoSeeded_RNG rng;
	Botan::Pipe pipe(get_cipher("AES-128/CBC", key, iv, Botan::DECRYPTION));
	pipe.process_msg(data);

	return pipe.read_all_as_string(0);
}

void AES::setAsBinaryVector(binary_vector_t serialized_vector) {
	binary_vector_t key_v = binary_vector_t(serialized_vector.begin(), serialized_vector.begin()+keySize());
	binary_vector_t iv_v = binary_vector_t(serialized_vector.begin()+keySize(), serialized_vector.begin()+vectorSize());
	key = Botan::OctetString(key_v);
	iv = Botan::OctetString(iv_v);
}

AES::binary_vector_t AES::toBinaryVector() const {
	binary_vector_t key_v = key.bits_of();
	binary_vector_t iv_v = iv.bits_of();

	binary_vector_t ret(key_v.begin(), key_v.end());
	ret.insert(ret.end(), iv_v.begin(), iv_v.end());
	return ret;
}

} /* namespace crypto */
} /* namespace p2pnet */
