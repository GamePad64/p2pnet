/*
 * CryptoTypes.h
 *
 *  Created on: 30.10.2012
 *      Author: gamepad
 */

#ifndef CRYPTOTYPES_H_
#define CRYPTOTYPES_H_

#include <botan/botan.h>
#include <vector>

namespace p2pnet {
namespace crypto {

typedef std::vector<Botan::byte> key_public_t;
typedef std::vector<Botan::byte, Botan::secure_allocator<Botan::byte> > key_private_t;

typedef struct {
	key_public_t key_public;
	key_private_t key_private;
} key_pair_t;

//! Type for storing results of hash-functions
typedef std::vector<Botan::byte, Botan::secure_allocator<Botan::byte> > hash_t;

//! Hamming distance of two hash_t
unsigned short hashDistance(hash_t hash1, hash_t hash2);
} /* namespace crypto */
} /* namespace p2pnet */

#endif /* CRYPTOTYPES_H_ */
