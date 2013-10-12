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

#ifndef HASH_H_
#define HASH_H_

#include "MathString.h"
#include <botan/keccak.h>

namespace p2pnet {
namespace crypto {

const short HASH_LENGTH = 224;

class PublicKeyDSA;

/**
 * Base class for hashing operations.
 * Now it represents Keccak algorithm, which was selected as SHA-3.
 */
class Hash : public MathString< Hash > {
	friend class MathString;
private:
	binary_vector_t hash;
	std::unique_ptr<Botan::Keccak_1600> hasher;
public:
	Hash();
	Hash(const Hash& rhash);
	Hash(Hash&& rhash);
	Hash(PublicKeyDSA dsa_pubkey);
	virtual ~Hash();

	Hash& operator = (const Hash& rhash);
	Hash& operator = (Hash&& rhash);

	bool operator == (const Hash& rhash);

	static Hash computeNew(const std::string& data);
	void compute(const std::string& data);

	bool check(const std::string& data);

	void setAsBinaryVector(binary_vector_t serialized_vector) {
		hash = serialized_vector;
	}
	binary_vector_t toBinaryVector() const {
		return hash;
	}

	unsigned short computeDistance(Hash rhash);

	std::vector<unsigned char> operator^(const Hash& lhash) const;
	bool operator<(const Hash& lhash) const;

	/**
	 * This function returns hashing algorithm name as Botan library does. It is needed for internal encryption operations.
	 * @return
	 */
	static std::string getAlgoName();
};

} /* namespace crypto */
} /* namespace p2pnet */
#endif /* HASH_H_ */
