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

#include <vector>
#include <string>
#include <botan/botan.h>

namespace p2pnet {
namespace crypto {

class Hash {
public:
	Hash();
	virtual ~Hash();

	typedef std::vector<Botan::byte, Botan::secure_allocator<Botan::byte> > hash_t;

	void fromBinaryVector(hash_t hash_vector);
	const hash_t toBinaryVector() const;

	void fromBinaryString(std::string hash_string);
	const std::string toBinaryString() const;

	void fromHex(std::string hex_string);
	const std::string toHex() const;

	void fromBase58(std::string hash_string);
	const std::string toBase58() const;

	void fromBase64(std::string hash_string);
	const std::string toBase64() const;

	unsigned short computeDistance(Hash rhash);
private:
	hash_t hash;
};

} /* namespace crypto */
} /* namespace p2pnet */
#endif /* HASH_H_ */
