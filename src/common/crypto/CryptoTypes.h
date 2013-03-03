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

#ifndef CRYPTOTYPES_H_
#define CRYPTOTYPES_H_

#include <botan/botan.h>
#include <vector>

namespace p2pnet {
namespace crypto {

typedef std::vector<Botan::byte> key_public_t;
typedef std::vector<Botan::byte, Botan::secure_allocator<Botan::byte> > key_private_t;

typedef struct {
	key_public_t key_public;
	key_private_t key_private;
} key_pair_t;

//! Type for storing results of hash-functions
typedef std::vector<Botan::byte, Botan::secure_allocator<Botan::byte> > hash_t;

//! Hamming distance of two hash_t
unsigned short hashDistance(hash_t hash1, hash_t hash2);
//! Returns Hex value of hash
std::string hashToHex(hash_t hash);
} /* namespace crypto */
} /* namespace p2pnet */

#endif /* CRYPTOTYPES_H_ */
