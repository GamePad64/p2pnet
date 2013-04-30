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

#ifndef PUBLICKEYDSA_H_
#define PUBLICKEYDSA_H_

#include <botan/ecdsa.h>
#include "MathString.h"

namespace p2pnet {
namespace crypto {

class PublicKeyDSA : public MathString<PublicKeyDSA> {
	Botan::ECDSA_PublicKey key_public;

	std::shared_ptr<Botan::ECDSA_PublicKey> getPublicKeyPtrFromBinaryVector(binary_vector_t serialized_vector);
protected:
	virtual const Botan::ECDSA_PublicKey& getPublicKey();

	PublicKeyDSA(Botan::ECDSA_PublicKey& botan_key);
public:
	PublicKeyDSA(binary_vector_t serialized_vector);
	virtual ~PublicKeyDSA();

	std::string encrypt(std::string data);

	bool verify(std::string data, std::string signature);
	bool verifyRaw(std::string data, std::string signature);

	bool validate();

	virtual void fromPEM(std::string pem);
	virtual std::string toPEM();

	virtual const binary_vector_t toBinaryVector() const;
};

} /* namespace crypto */
} /* namespace p2pnet */
#endif /* PUBLICKEYDSA_H_ */
