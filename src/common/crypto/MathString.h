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

#ifndef MATHSTRING_H_
#define MATHSTRING_H_

#include "base_convert.h"

#include <vector>
#include <string>
#include <botan/botan.h>

namespace p2pnet {
namespace crypto {

template< class T >
class MathString {
public:
	typedef std::vector< Botan::byte, Botan::secure_allocator< Botan::byte > > binary_vector_t;

protected:
	MathString(const binary_vector_t bv) {
	}

public:
	virtual ~MathString() {
	}

	static T fromBinaryVector(binary_vector_t serialized_vector) {
		return T(serialized_vector);
	}
	virtual const binary_vector_t toBinaryVector() const = 0;

	static T fromBinaryString(std::string serialized_string) {
		return fromBinaryVector(binary_vector_t(serialized_string.begin(), serialized_string.end()));
	}
	const std::string toBinaryString() const {
		binary_vector_t bv = toBinaryVector();
		std::string s = std::string(bv.begin(), bv.end());
		return s;
	}

	static T fromHex(std::string hex_string) {
		return fromBinaryString(decodeFromHex(hex_string));
	}
	const std::string toHex() const {
		return encodeToHex(toBinaryString());
	}

	static T fromBase58(std::string b58_string) {
		return fromBinaryString(decodeFromBase58(b58_string));
	}
	const std::string toBase58() const {
		return encodeToBase58(toBinaryString());
	}

	static T fromBase64(std::string b64_string) {
		return fromBinaryString(decodeFromBase64(b64_string));
	}
	const std::string toBase64() const {
		return encodeToBase64(toBinaryString());
	}
};

} /* namespace crypto */
} /* namespace p2pnet */
#endif /* MATHSTRING_H_ */
