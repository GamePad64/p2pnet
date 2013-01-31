/*
 * CipherSet.h
 *
 *  Created on: 30.10.2012
 *      Author: gamepad
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
};

} /* namespace crypto */
} /* namespace p2pnet */

#endif /* CIPHERSET_H_ */
