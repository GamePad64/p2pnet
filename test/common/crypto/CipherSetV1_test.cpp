/*
 * CipherSetV1_test.cpp
 *
 *  Created on: 07.01.2013
 *      Author: gamepad
 */

/*
 * UDPSocket_test.cpp
 *
 *  Created on: 07.01.2013
 *      Author: gamepad
 */

#include "../../../src/common/crypto/CipherSetV1.h"
#include "gtest/gtest.h"

namespace p2pnet {
namespace test {

crypto::CipherSetV1 ciphersetv1;

TEST(Crypto_Base64, Base64_encode){
	ASSERT_EQ(ciphersetv1.encodeToBase64("Hello, World!"), "SGVsbG8sIFdvcmxkIQ==");
	ASSERT_EQ(ciphersetv1.encodeToBase64("Base64 encoded string"), "QmFzZTY0IGVuY29kZWQgc3RyaW5n");
}

TEST(Crypto_Base64, Base64_decode){
	ASSERT_EQ(ciphersetv1.decodeFromBase64("SGVsbG8sIFdvcmxkIQ=="), "Hello, World!");
	ASSERT_EQ(ciphersetv1.decodeFromBase64("QmFzZTY0IGRlY29kZWQgc3RyaW5n"), "Base64 decoded string");
}

TEST(CryptoSetV1_Hash, computeHash){
	crypto::hash_t hash = ciphersetv1.computeHash("Hello, World!");
	ASSERT_EQ("7adlV2yExgDtf12XUQ6ScDth9SFd7yoWEDf9ndH1tu1Phs5GBzwOPzS1LeAonpxhh5j/+d1LG/4DW9uGRfxuNw==", ciphersetv1.encodeToBase64(std::string(hash.begin(), hash.end())));
}

}
}
