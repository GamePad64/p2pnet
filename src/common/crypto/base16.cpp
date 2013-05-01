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

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

namespace p2pnet {
namespace crypto {

/**
 * C++98 guarantees that '0', '1', ... '9' are consecutive.
 * It only guarantees that 'a' ... 'f' and 'A' ... 'F' are
 * in increasing order, but the only two alternative encodings
 * of the basic source character set that are still used by
 * anyone today (ASCII and EBCDIC) make them consecutive.
 */
unsigned char hexval(char c){
	if ('0' <= c && c <= '9')
		return c - '0';
	else if ('A' <= c && c <= 'F')
		return c - 'A' + 10;
	else /*if ('a' <= c && c <= 'f')*/
		return c - 'a' + 10;
	//else
		//EXCEPTION: This string is NOT HEX.
}

std::string encodeToHex(std::string data){
	std::ostringstream hash_ss;

	hash_ss << std::hex << std::uppercase << std::setfill('0');
	for( int twobytes : data ) {
		hash_ss << std::setw(2) << twobytes;
	}

	return hash_ss.str();
}

std::string decodeFromHex(std::string hex_string){
	std::string data;
	data.reserve(hex_string.length() / 2);

	for (auto p = hex_string.begin(); p != hex_string.end(); p++){
		unsigned char c = hexval(*p);
		p++;
		if (p == hex_string.end()){
			break; //EXCEPTION: Incomplete last digit, hex string is corrupted.
		}
		c = (c << 4) + hexval(*p);
		data.push_back(c);
	}

	return data;
}

}
}
