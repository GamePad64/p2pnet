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

#ifndef ECDH_H_
#define ECDH_H_

#include <botan/ecdh.h>
#include "MathString.h"
#include <mutex>

namespace p2pnet {
namespace crypto {

const std::string ecdh_curve = "secp256r1";

class ECDH : public MathString<ECDH> {
	std::unique_ptr<Botan::ECDH_PrivateKey> key_private;

public:
	/**
	 * Generic ECDH constructor. This creates empty ECDH key pair. Any call to function that requires ECDH not to be
	 * empty initializes it, generating new key pair.
	 */
	ECDH();
	ECDH(const ECDH& rvalue);
	ECDH(ECDH&& rvalue);
	ECDH(binary_vector_t serialized_vector);
	virtual ~ECDH();

	ECDH& operator =(const ECDH& rvalue);
	ECDH& operator =(ECDH&& rvalue);

	static ECDH generateNewKey();
	void generateKey();

	std::string deriveSymmetricKey(size_t key_length, std::string other_pubkey, std::string session_param) const;
	std::string derivePublicKey() const;

	void setAsBinaryVector(binary_vector_t serialized_vector);
	virtual binary_vector_t toBinaryVector() const;
};

} /* namespace crypto */
} /* namespace p2pnet */
#endif /* ECDH_H_ */
