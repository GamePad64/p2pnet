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

#ifndef CIPHERSETV1_H_
#define CIPHERSETV1_H_

#include "CipherSet.h"
#include <botan/keccak.h>
#include <botan/rsa.h>
#include <string>

namespace p2pnet {
namespace crypto {

/** CipherSetV1:
 * 	uses Keccak as hash function
 * 	uses RSA as asymmetric encryption algorithm
 * 	will use DSA as signature algorithm
 */
class CipherSetV1: public p2pnet::crypto::CipherSet {
private:
	Botan::AutoSeeded_RNG rng;
public:
	CipherSetV1();
	virtual ~CipherSetV1();

	const short KECCAK_LENGTH = 512;
	const short RSA_LENGTH = 4096;

	virtual hash_t computeHash(const std::string& data);
	virtual bool checkHash(const std::string& data, const hash_t& hash);
	virtual key_pair_t generateKeyPair();
	virtual bool checkKeyPair(key_pair_t key_pair);


	virtual std::string privKeyToPEM(key_private_t key_private);
	virtual std::string pubKeyToPEM(key_public_t key_public);

	// TODO PEM serialization
	//virtual key_private_t PEMToPrivKey(std::string key_pem);
	//virtual key_public_t PEMToPubKey(std::string key_pem);

	// TODO DSA signature
	// TODO Encryptions!

	virtual std::string encodeToBase64(std::string data);
	virtual std::string decodeFromBase64(std::string base64);


	const std::string base58_alphabet = "123456789abcdefghijkmnopqrstuvwxyzABCDEFGHJKLMNPQRSTUVWXYZ";
	virtual std::string encodeToBase58(std::string data);
	virtual std::string decodeFromBase58(std::string base58);
};

} /* namespace crypto */
} /* namespace p2pnet */
#endif /* CIPHERSETV1_H_ */
