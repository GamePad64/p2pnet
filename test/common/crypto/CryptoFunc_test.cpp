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

#include "../../../src/common/crypto/CryptoTypes.h"
#include "gtest/gtest.h"

namespace p2pnet {
namespace test {

TEST(HammingDistance, NullHash){
	crypto::hash_t h1, h2;
	ASSERT_EQ(0, crypto::hashDistance(h1, h2));
}

TEST(HammingDistance, EqualHash){
	std::string s1 = "Hello";
	std::string s2 = "Hello";

	crypto::hash_t h1;
	crypto::hash_t h2;
	ASSERT_EQ(0, crypto::hashDistance(h1, h2));
}

TEST(HammingDistance, Distance1){
	std::string s1 = "Helln";
	std::string s2 = "Hello";
	std::string s3 = "Hellq";

	crypto::hash_t h1(s1.begin(), s1.end());
	crypto::hash_t h2(s2.begin(), s2.end());
	crypto::hash_t h3(s3.begin(), s3.end());
	ASSERT_EQ(1, crypto::hashDistance(h1, h2));
	ASSERT_EQ(4, crypto::hashDistance(h2, h3));
	ASSERT_EQ(5, crypto::hashDistance(h1, h3));
}

}
}
