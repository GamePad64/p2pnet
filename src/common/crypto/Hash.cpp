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

bool Hash::operator == (const Hash& rhash) const{
	return hash == rhash.hash;
}

Hash::operator bool(){
	return !(hash.empty());
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

Hash Hash::operator^(const Hash& rhash) const {
	binary_vector_t this_v = this->toBinaryVector();
	binary_vector_t right_v = rhash.toBinaryVector();

	auto max_size = std::max(this_v.size(), right_v.size());
	this_v.resize(max_size);
	right_v.resize(max_size);

	binary_vector_t result_v(max_size);

	for(size_t i; i < max_size; i++) {
		result_v[i] = this_v[i] ^ right_v[i];
	}
	return result_v;
}

bool Hash::operator <(const Hash& lhash) const {
	return std::string(hash.begin(), hash.end()) <
			std::string(lhash.hash.begin(), lhash.hash.end());
}

void Hash::clear(){
	hash.clear();
}

} /* namespace crypto */
} /* namespace p2pnet */
