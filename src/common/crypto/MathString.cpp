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

#include "MathString.h"
#include "base58.h"
#include "base64.h"
#include <sstream>
#include <iomanip>

namespace p2pnet {
namespace crypto {

MathString::MathString() {}
MathString::~MathString() {}

void MathString::fromBinaryString(std::string hash_string) {
	fromBinaryVector(binary_vector_t(hash_string.begin(), hash_string.end()));
}

const std::string MathString::toBinaryString() const {
	std::string s = std::string(toBinaryVector().begin(), toBinaryVector().end());
	return s;
}

void MathString::fromHex(std::string hex_string) {
	//FIXME: Well, later.
	// Use strtoul(), Luke! It is in <cstdlib>, you know :)
}

const std::string MathString::toHex() const {
	std::ostringstream hash_ss;

	hash_ss << std::hex << std::uppercase << std::setfill('0');
	for( int twobytes : toBinaryString() ) {
		hash_ss << std::setw(2) << twobytes;
	}

	return hash_ss.str();
}

void MathString::fromBase58(std::string hash_string) {
	fromBinaryString(decodeFromBase58(hash_string));
}

const std::string MathString::toBase58() const {
	return encodeToBase58(toBinaryString());
}

void MathString::fromBase64(std::string hash_string) {
	fromBinaryString(decodeFromBase58(hash_string));
}

const std::string MathString::toBase64() const {
	return encodeToBase64(toBinaryString());
}

} /* namespace crypto */
} /* namespace p2pnet */
