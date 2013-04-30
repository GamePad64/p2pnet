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

/**
 * Base class for hashing operations.
 * Now it represents Keccak algorithm, which was selected as SHA-3.
 */
class Hash : public MathString< Hash > {
	friend class MathString;
private:
	binary_vector_t hash;
	static Botan::Keccak_1600 hasher;

	friend class PublicKeyDSA;
	friend class PrivateKeyDSA;
	/**
	 * This function returns hashing algorithm name as Botan library does. It is needed for internal encryption operations.
	 * @return
	 */
	static std::string getAlgoName();
protected:
	Hash(const binary_vector_t serialized_vector);
public:
	virtual ~Hash();

	static Hash compute(std::string data);
	bool check(std::string data);

	const binary_vector_t toBinaryVector() const {
		return hash;
	}

	unsigned short computeDistance(Hash rhash);
};

} /* namespace crypto */
} /* namespace p2pnet */
#endif /* HASH_H_ */
