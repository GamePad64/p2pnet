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
	MathString() {}

public:
	virtual ~MathString() {
	}

	virtual void setAsBinaryVector(binary_vector_t serialized_vector) = 0;
	static T fromBinaryVector(binary_vector_t serialized_vector) {
		return T(serialized_vector);
	}
	virtual binary_vector_t toBinaryVector() const = 0;

	void setAsBinaryString(std::string serialized_string){
		setAsBinaryVector(binary_vector_t(serialized_string.begin(), serialized_string.end()));
	}
	static T fromBinaryString(std::string serialized_string) {
		T new_string;
		new_string.setAsBinaryString(serialized_string);
		return new_string;
	}
	std::string toBinaryString() const {
		binary_vector_t bv = toBinaryVector();
		std::string s = std::string(bv.begin(), bv.end());
		return s;
	}

	void setAsHex(std::string hex_string){
		setAsBinaryString(decodeFromHex(hex_string));
	}
	static T fromHex(std::string hex_string) {
		T new_string;
		new_string.setAsHex(hex_string);
		return new_string;
	}
	std::string toHex() const {
		return encodeToHex(toBinaryString());
	}

	void setAsBase58(std::string b58_string){
		setAsBinaryString(decodeFromBase58(b58_string));
	}
	static T fromBase58(std::string b58_string) {
		T new_string;
		new_string.setAsBase58(b58_string);
		return new_string;
	}
	std::string toBase58() const {
		return encodeToBase58(toBinaryString());
	}

	void setAsBase64(std::string b64_string){
		setAsBinaryString(decodeFromBase64(b64_string));
	}
	static T fromBase64(std::string b64_string) {
		T new_string;
		new_string.setAsBase64(b64_string);
		return new_string;
	}
	std::string toBase64() const {
		return encodeToBase64(toBinaryString());
	}
};

} /* namespace crypto */
} /* namespace p2pnet */
#endif /* MATHSTRING_H_ */
