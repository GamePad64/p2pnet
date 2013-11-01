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
#include "PublicKeyDSA.h"
#include <list>

namespace p2pnet {
namespace crypto {

std::string Hash::getAlgoName() {
	Hash h;
	return h.hasher->name();
}

Hash::Hash() {
	hasher = std::unique_ptr<Botan::Keccak_1600>(new Botan::Keccak_1600(HASH_LENGTH));
}
Hash::Hash(const Hash& rhash){
	hash = rhash.hash;
}
Hash::Hash(Hash&& rhash){
	std::swap(hash, rhash.hash);
	std::swap(hasher, rhash.hasher);
}
Hash::Hash(PublicKeyDSA dsa_pubkey) : Hash() {
	compute(dsa_pubkey.toBinaryString());
}
Hash::Hash(const std::string& data) : Hash() {
	compute(data);
}

Hash::~Hash() {}

Hash& Hash::operator =(const Hash& rhash) {
	hash = rhash.hash;
	return *this;
}

Hash& Hash::operator =(Hash&& rhash) {
	std::swap(hash, rhash.hash);
	std::swap(hasher, rhash.hasher);
	return *this;
}

bool Hash::operator == (const Hash& rhash){
	return hash == rhash.hash;
}

Hash Hash::computeNew(const std::string& data) {
	Hash h;
	h.compute(data);
	return h;
}

void Hash::compute(const std::string& data) {
	setAsBinaryVector(hasher->process(data));
}

bool Hash::check(const std::string& data) {
	Hash h;
	h.compute(data);

	return (h.toBinaryString() == this->toBinaryString());
}

unsigned short Hash::computeDistance(const Hash& rhash) const {
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

std::vector<unsigned char> Hash::operator^(const Hash& rhash) const {
	std::list<unsigned char> result_v;
	binary_vector_t this_v = this->toBinaryVector();
	binary_vector_t right_v = rhash.toBinaryVector();

	auto this_i = this_v.begin();
	auto left_i = right_v.begin();
	while( (this_i != this_v.end()) && (left_i != right_v.end()) ){
		result_v.push_back( (*this_i) ^ (*left_i) );
		this_i++;
		left_i++;
	}
	return ( std::vector<unsigned char>(result_v.begin(), result_v.end()) );
}

bool Hash::operator <(const Hash& lhash) const {
	return hash < lhash.hash;
}

} /* namespace crypto */
} /* namespace p2pnet */
