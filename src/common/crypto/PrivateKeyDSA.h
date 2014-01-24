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

#ifndef PRIVATEKEYDSA_H_
#define PRIVATEKEYDSA_H_

#include "MathString.h"
#include <botan/ecdsa.h>

namespace p2pnet {
namespace crypto {

const std::string dsa_curve = "secp256r1";
constexpr short dsa_bit_length = 256;
constexpr short dsa_byte_length = dsa_bit_length/8;

class PublicKeyDSA;

class PrivateKeyDSA : virtual public MathString<PrivateKeyDSA> {
	friend class PublicKeyDSA;
	std::unique_ptr<Botan::ECDSA_PrivateKey> key_private;
public:
	PrivateKeyDSA();
	PrivateKeyDSA(const PrivateKeyDSA& rvalue);
	PrivateKeyDSA(PrivateKeyDSA&& rvalue);
	virtual ~PrivateKeyDSA();

	PrivateKeyDSA& operator =(const PrivateKeyDSA& rvalue);
	PrivateKeyDSA& operator =(PrivateKeyDSA&& rvalue);

	static PrivateKeyDSA generateNewKey();
	void generateKey();

	std::string decrypt(std::string enc_data);

	std::string sign(std::string data) const;
	std::string signRaw(std::string data) const;

	virtual void fromPEM(std::string pem);
	virtual std::string toPEM();

	virtual void fromBER(binary_vector_t ber_encoded);
	virtual binary_vector_t toBER();

	void setAsBinaryVector(binary_vector_t serialized_vector);
	virtual binary_vector_t toBinaryVector() const;

	PublicKeyDSA derivePublicKey() const;
};

} /* namespace crypto */
} /* namespace p2pnet */
#endif /* PRIVATEKEYDSA_H_ */
