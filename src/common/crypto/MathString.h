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

#include <vector>
#include <string>
#include <botan/botan.h>

namespace p2pnet {
namespace crypto {

class MathString {
public:
	MathString();
	virtual ~MathString();

	typedef std::vector<Botan::byte, Botan::secure_allocator<Botan::byte> > binary_vector_t;

	virtual void fromBinaryVector(binary_vector_t hash_vector) = 0;
	virtual const binary_vector_t toBinaryVector() const = 0;

	virtual void fromBinaryString(std::string hash_string);
	virtual const std::string toBinaryString() const;

	virtual void fromHex(std::string hex_string);
	virtual const std::string toHex() const;

	virtual void fromBase58(std::string hash_string);
	virtual const std::string toBase58() const;

	virtual void fromBase64(std::string hash_string);
	virtual const std::string toBase64() const;
};

} /* namespace crypto */
} /* namespace p2pnet */
#endif /* MATHSTRING_H_ */
