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

#ifndef CIPHERSET_H_
#define CIPHERSET_H_

#include "CryptoTypes.h"
#include <string>
#include <vector>
#include <botan/botan.h>

namespace p2pnet {
namespace crypto {

class CipherSet {
public:
	CipherSet(){};
	virtual ~CipherSet(){};

	// Cryptographic functions
	//! Computes hash from string
	virtual hash_t computeHash(const std::string& data) = 0;
	//! Checks if hash is hash(data)
	virtual bool checkHash(const std::string& data, const hash_t& hash) = 0;
	//! Generates key pair in BER format
	virtual key_pair_t generateKeyPair() = 0;
	virtual bool checkKeyPair(key_pair_t key_pair) = 0;

	// PEM generators
	virtual std::string privKeyToPEM(key_private_t key_private) = 0;
	virtual std::string pubKeyToPEM(key_public_t key_public) = 0;

	//virtual key_private_t PEMToPrivKey(std::string key_pem) = 0;
	//virtual key_public_t PEMToPubKey(std::string key_pem) = 0;

	// Base64 operations
	virtual std::string encodeToBase64(std::string data) = 0;
	virtual std::string decodeFromBase64(std::string base64) = 0;
	// Base58 operations
	virtual std::string encodeToBase58(std::string data) = 0;
	virtual std::string decodeFromBase58(std::string base64) = 0;
};

} /* namespace crypto */
} /* namespace p2pnet */

#endif /* CIPHERSET_H_ */
