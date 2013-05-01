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

#include "crc32.h"
#include <botan/crc32.h>

namespace p2pnet {
namespace crypto {

uint32_t computeCRC32(std::string data) {
	Botan::CRC32 hasher;
	auto crc32_v = hasher.process(data);
	uint32_t crc32 =
			(uint32_t)crc32_v[0] << 24 |
			(uint32_t)crc32_v[1] << 16 |
			(uint32_t)crc32_v[2] << 8  |
			(uint32_t)crc32_v[3];
	return crc32;
}

}
}
