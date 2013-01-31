/*
 * CipherSetV1.cpp
 *
 *  Created on: 30.10.2012
 *      Author: gamepad
 */

#include "CipherSetV1.h"

namespace p2pnet {
namespace crypto {

CipherSetV1::CipherSetV1() {}
CipherSetV1::~CipherSetV1() {}

hash_t CipherSetV1::computeHash(const std::string& data) {
	Botan::Keccak_1600 hasher(KECCAK_LENGTH);
	return hasher.process(data);
}

bool CipherSetV1::checkHash(const std::string& data, const hash_t& hash) {
	return (this->computeHash(data) == hash);
}

key_pair_t CipherSetV1::generateKeyPair() {
	Botan::RSA_PrivateKey key(rng, RSA_LENGTH);

	key_pair_t keys;
	keys.key_public = Botan::X509::BER_encode(key);
	keys.key_private = Botan::PKCS8::BER_encode(key);
	return keys;
}

bool CipherSetV1::checkKeyPair(key_pair_t key_pair) {
	if(key_pair.key_private.empty() || key_pair.key_public.empty()){
		return false;
	}
	Botan::DataSource_Memory botan_source_private(key_pair.key_private);
	Botan::RSA_PrivateKey* key_private_botan = dynamic_cast<Botan::RSA_PrivateKey*>(Botan::PKCS8::load_key(botan_source_private, rng));

	Botan::DataSource_Memory botan_source_public(key_pair.key_public);
	Botan::RSA_PublicKey* key_public_botan = dynamic_cast<Botan::RSA_PublicKey*>(Botan::X509::load_key(botan_source_public));

	if(Botan::X509::BER_encode(*key_private_botan) != Botan::X509::BER_encode(*key_public_botan)){
		return false;
	}

	return key_private_botan->check_key(rng, false);
}

std::string CipherSetV1::privKeyToPEM(key_private_t key_private) {
	Botan::DataSource_Memory botan_source(key_private);
	Botan::RSA_PrivateKey* key_private_botan = dynamic_cast<Botan::RSA_PrivateKey*>(Botan::PKCS8::load_key(botan_source, rng));
	return Botan::PKCS8::PEM_encode(*key_private_botan);
}

std::string CipherSetV1::pubKeyToPEM(key_public_t key_public) {
	Botan::DataSource_Memory botan_source(key_public);
	Botan::RSA_PublicKey* key_public_botan = dynamic_cast<Botan::RSA_PublicKey*>(Botan::X509::load_key(botan_source));
	return Botan::X509::PEM_encode(*key_public_botan);
}

/*key_private_t CipherSetV1::PEMToPrivKey(std::string key_pem) {
}

key_public_t CipherSetV1::PEMToPubKey(std::string key_pem) {
}*/

std::string CipherSetV1::encodeToBase64(std::string data) {
	Botan::Pipe pipe(new Botan::Base64_Encoder);
	pipe.process_msg(data);
	return pipe.read_all_as_string(0);
}

std::string CipherSetV1::decodeFromBase64(std::string base64) {
	Botan::Pipe pipe(new Botan::Base64_Decoder);
	pipe.process_msg(base64);
	return pipe.read_all_as_string(0);
}

} /* namespace crypto */
} /* namespace p2pnet */
