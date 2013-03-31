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

#ifndef BASE58_H_
#define BASE58_H_

#include <botan/bigint.h>
#include <string>
#include <deque>
#include "CipherSetV1.h"

namespace p2pnet {
namespace crypto {

std::string base58_alphabet = "123456789abcdefghijkmnopqrstuvwxyzABCDEFGHJKLMNPQRSTUVWXYZ";

std::string CipherSetV1::encodeToBase58(std::string data) {
	Botan::BigInt big_data = Botan::BigInt::decode(std::vector<unsigned char>(data.begin(), data.end()));

	Botan::BigInt base_count = base58_alphabet.length();
	std::deque<char> result;

	while(big_data >= base_count){
		Botan::BigInt div = big_data / base_count;
		Botan::BigInt mod = big_data % base_count;
		result.push_front(base58_alphabet[mod.to_u32bit()]);
		big_data = div;
	}

	if(big_data > 0){
		result.push_front(base58_alphabet[big_data.to_u32bit()]);
	}

	return std::string(result.begin(), result.end());
}

std::string CipherSetV1::decodeFromBase58(std::string base58) {
	int len = base58.length();

	Botan::BigInt big_data = 0;
	Botan::BigInt multi = 1;

	for(int i = len-1; i >= 0; i--){
		big_data += multi * base58_alphabet.find(base58[i]);
		multi *= base58_alphabet.length();
	}

	std::vector<unsigned char> decoded = Botan::BigInt::encode(big_data);
	return std::string(decoded.begin(), decoded.end());
}

}
}

#endif /* BASE58_H_ */
