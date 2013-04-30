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

#include <botan/botan.h>

namespace p2pnet {
namespace crypto {

std::string encodeToBase64(std::string data) {
	Botan::Pipe pipe(new Botan::Base64_Encoder);
	pipe.process_msg(data);
	return pipe.read_all_as_string(0);
}

std::string decodeFromBase64(std::string base64) {
	Botan::Pipe pipe(new Botan::Base64_Decoder);
	pipe.process_msg(base64);
	return pipe.read_all_as_string(0);
}

}
}
