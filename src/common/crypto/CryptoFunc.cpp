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

#include "CryptoTypes.h"
#include <sstream>
#include <string>
#include <iomanip>

namespace p2pnet {
namespace crypto {

unsigned short hashDistance(hash_t hash1, hash_t hash2){
	unsigned short distance = 0;

	auto char1 = hash1.begin();
	auto char2 = hash2.begin();

	unsigned char comp_value;

	while(char1 != hash1.end() && char2 != hash2.end()){
		comp_value = (*char1) ^ (*char2);
		while(comp_value){
			++distance;
			comp_value &= comp_value - 1;
		}
		++char1;
		++char2;
	}

	return distance;
}

std::string hashToHex(hash_t hash){
	std::ostringstream hash_ss;

	hash_ss << std::hex << std::uppercase << std::setfill('0');
	for( int twobytes : hash ) {
		hash_ss << std::setw(2) << twobytes;
	}

	return hash_ss.str();
}


} /* namespace crypto */
} /* namespace p2pnet */
