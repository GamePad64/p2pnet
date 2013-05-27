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
//#include "base58.h"
//#include "base64.h"
#include <sstream>
#include <iomanip>
#include <deque>

namespace p2pnet {
namespace crypto {

Botan::Keccak_1600 Hash::hasher(HASH_LENGTH);

std::string Hash::getAlgoName() {
	return hasher.name();
}

Hash::Hash(const binary_vector_t serialized_vector) :
		MathString< Hash >::MathString(serialized_vector) {
	hash = serialized_vector;
}

Hash::~Hash() {
}

Hash Hash::compute(std::string data) {
	return Hash(hasher.process(data));
}

bool Hash::check(std::string data) {
	Hash h = Hash::compute(data);

	return (h.toBinaryString() == this->toBinaryString());
}

unsigned short Hash::computeDistance(Hash rhash) {
	unsigned short distance = 0;

	binary_vector_t rhash_vector = rhash.toBinaryVector();

	auto char1 = hash.begin();
	auto char2 = rhash_vector.begin();

	unsigned char comp_value;

	while (char1 != hash.end() && char2 != rhash_vector.end()) {
		comp_value = (*char1) ^ (*char2);
		while (comp_value) {
			++distance;
			comp_value &= comp_value - 1;
		}
		++char1;
		++char2;
	}

	return distance;
}

std::vector<unsigned char> Hash::operator^(const Hash& lhash) const {
	std::deque<unsigned char> result_v;
	binary_vector_t this_v = this->toBinaryVector();
	binary_vector_t left_v = lhash.toBinaryVector();

	auto this_i = this_v.begin();
	auto left_i = left_v.begin();
	while( (this_i != this_v.end()) && (left_i != left_v.end()) ){
		result_v.push_back( (*this_i) ^ (*left_i) );
		this_i++;
		left_i++;
	}
	return ( std::vector<unsigned char>(result_v.begin(), result_v.end()) );
}

} /* namespace crypto */
} /* namespace p2pnet */
