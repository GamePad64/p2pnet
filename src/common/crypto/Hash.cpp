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

#include "Hash.h"
#include "base58.h"
#include "base64.h"
#include <sstream>
#include <iomanip>

namespace p2pnet {
namespace crypto {

Hash::Hash() {}
Hash::~Hash() {}

void Hash::fromBinaryVector(hash_t hash_vector){
	hash = hash_vector;
}

const Hash::hash_t Hash::toBinaryVector() const {
	return hash;
}

void Hash::fromBinaryString(std::string hash_string) {
	hash = hash_t(hash_string.begin(), hash_string.end());
}

const std::string Hash::toBinaryString() const {
	std::string s = std::string(hash.begin(), hash.end());
	return s;
}

void Hash::fromHex(std::string hex_string) {
	//FIXME: Well, later.
	// Use strtoul(), Luke! It is in <cstdlib>, you know :)
}

const std::string Hash::toHex() const {
	std::ostringstream hash_ss;

	hash_ss << std::hex << std::uppercase << std::setfill('0');
	for( int twobytes : hash ) {
		hash_ss << std::setw(2) << twobytes;
	}

	return hash_ss.str();
}

void Hash::fromBase58(std::string hash_string) {
	fromBinaryString(decodeFromBase58(hash_string));
}

const std::string Hash::toBase58() const {
	return encodeToBase58(toBinaryString());
}

void Hash::fromBase64(std::string hash_string) {
	fromBinaryString(decodeFromBase58(hash_string));
}

const std::string Hash::toBase64() const {
	return encodeToBase64(toBinaryString());
}

unsigned short Hash::computeDistance(Hash rhash) {
	unsigned short distance = 0;

	hash_t rhash_vector = rhash.toBinaryVector();

	auto char1 = hash.begin();
	auto char2 = rhash_vector.begin();

	unsigned char comp_value;

	while(char1 != hash.end() && char2 != rhash_vector.end()){
		comp_value = (*char1) ^ (*char2);
		while(comp_value){
			++distance;
			comp_value &= comp_value - 1;
		}
		++char1;
		++char2;
	}

	return distance;
}

} /* namespace crypto */
} /* namespace p2pnet */
