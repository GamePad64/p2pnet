/*
 * CryptoFunc.cpp
 *
 *  Created on: 17.01.2013
 *      Author: gamepad
 */

#include "CryptoTypes.h"

namespace p2pnet {
namespace crypto {

unsigned short hashDistance(hash_t hash1, hash_t hash2){
	unsigned short distance = 0;

	auto char1 = hash1.begin();
	auto char2 = hash2.begin();

	unsigned char comp_value;

	while(char1 != hash1.end() && char2 != hash2.end()){
		comp_value = (*char1) ^ (*char2);
		while(comp_value){
			++distance;
			comp_value &= comp_value - 1;
		}
		++char1;
		++char2;
	}

	return distance;
}


} /* namespace crypto */
} /* namespace p2pnet */
