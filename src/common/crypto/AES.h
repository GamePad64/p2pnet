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
#ifndef AES_H_
#define AES_H_

#include "MathString.h"
#include <botan/aes.h>

namespace p2pnet {
namespace crypto {

class AES : MathString<AES> {
	Botan::SymmetricKey key;
	Botan::InitializationVector iv;
public:
	AES();
	virtual ~AES();

	std::string encrypt(std::string data);
	std::string decrypt(std::string data);

	void setAsBinaryVector(binary_vector_t serialized_vector);
	virtual binary_vector_t toBinaryVector() const;
};

} /* namespace crypto */
} /* namespace p2pnet */

#endif /* AES_H_ */
