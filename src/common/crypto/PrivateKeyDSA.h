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

#include "PublicKeyDSA.h"

namespace p2pnet {
namespace crypto {

const std::string curve = "secp256r1";

class PrivateKeyDSA : virtual public MathString<PrivateKeyDSA> {
	Botan::ECDSA_PrivateKey key_private;

	std::shared_ptr<Botan::ECDSA_PrivateKey> getPrivateKeyPtrFromBinaryVector(binary_vector_t serialized_vector);
protected:
	virtual const Botan::ECDSA_PrivateKey& getPrivateKey();

	PrivateKeyDSA(Botan::ECDSA_PrivateKey& botan_key);
public:
	PrivateKeyDSA(binary_vector_t serialized_vector);
	virtual ~PrivateKeyDSA();

	static PrivateKeyDSA generate();

	std::string decrypt(std::string enc_data);

	std::string sign(std::string data);
	std::string signRaw(std::string data);

	virtual void fromPEM(std::string pem);
	virtual std::string toPEM();

	virtual const binary_vector_t toBinaryVector() const;

	PublicKeyDSA derivePublicKey();
};

} /* namespace crypto */
} /* namespace p2pnet */
#endif /* PRIVATEKEYDSA_H_ */
