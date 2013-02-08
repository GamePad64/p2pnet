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

#include "../../src/common/Version.h"
#include "gtest/gtest.h"

namespace p2pnet {
namespace test {

TEST(Version, To_From_String_Variantless){
	Version v(2, 3, 4);
	ASSERT_EQ("2.3.4", v.toString());
	Version vv;
	vv.fromString("3.4.5");
	ASSERT_EQ("3.4.5", vv.toString());
}

TEST(Version, To_From_String_Variant){
	Version v(2, 3, 4, "devel");
	ASSERT_EQ("2.3.4-devel", v.toString());
	Version vv;
	vv.fromString("3.4.5-ubuntu");
	ASSERT_EQ("3.4.5-ubuntu", vv.toString());
}

}
}
