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
