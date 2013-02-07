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

#include "gtest/gtest.h"
#include "../../src/common/Config.h"

namespace p2pnet {
namespace test {

Config config;

TEST(Config, Check_Default){
	config.loadFromFile();
	ASSERT_EQ(config.getDefaults().net_udp_v4_port, config.getConfig().net_udp_v4_port);
	ASSERT_EQ(config.getDefaults().net_udp_v4_bind, config.getConfig().net_udp_v4_bind);
	ASSERT_EQ(config.getDefaults().net_udp_v4_lpd_timer, config.getConfig().net_udp_v4_lpd_timer);
	ASSERT_EQ(config.getDefaults().net_udp_v4_lpd_address, config.getConfig().net_udp_v4_lpd_address);
	ASSERT_EQ(config.getDefaults().net_udp_v4_lpd_port, config.getConfig().net_udp_v4_lpd_port);
	ASSERT_EQ(config.getDefaults().net_udp_v4_lpd_bind, config.getConfig().net_udp_v4_lpd_bind);

	ASSERT_EQ(config.getDefaults().net_udp_v6_port, config.getConfig().net_udp_v6_port);
	ASSERT_EQ(config.getDefaults().net_udp_v6_bind, config.getConfig().net_udp_v6_bind);
	ASSERT_EQ(config.getDefaults().net_udp_v6_lpd_timer, config.getConfig().net_udp_v6_lpd_timer);
	ASSERT_EQ(config.getDefaults().net_udp_v6_lpd_address, config.getConfig().net_udp_v6_lpd_address);
	ASSERT_EQ(config.getDefaults().net_udp_v6_lpd_port, config.getConfig().net_udp_v6_lpd_port);
	ASSERT_EQ(config.getDefaults().net_udp_v6_lpd_bind, config.getConfig().net_udp_v6_lpd_bind);
}

}
}
