/*
 * CipherSetV1.h
 *
 *  Created on: 30.10.2012
 *      Author: gamepad
 */

#ifndef CIPHERSETV1_H_
#define CIPHERSETV1_H_

#include "CipherSet.h"
#include <botan/keccak.h>
#include <botan/rsa.h>

namespace p2pnet {
namespace crypto {

/** CipherSetV1:
 * 	uses Keccak as hash function
 * 	uses RSA as asymmetric encryption algorithm
 *
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

	//virtual key_private_t PEMToPrivKey(std::string key_pem);
	//virtual key_public_t PEMToPubKey(std::string key_pem);

	virtual std::string encodeToBase64(std::string data);
	virtual std::string decodeFromBase64(std::string base64);
};

} /* namespace crypto */
} /* namespace p2pnet */
#endif /* CIPHERSETV1_H_ */
