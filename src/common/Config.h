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

#ifndef CONFIG_H_
#define CONFIG_H_

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace p2pnet {
class Config {
	boost::property_tree::ptree pt;
	mutable std::string config_filename;

	/**
	 * Returns full path to configuration file.
	 * @note Platform-dependent. Also, if superuser, it tries to save config system-wide.
	 * @return
	 */
	const std::string getConfigFilename(bool force = false) const;
public:
	Config();
	virtual ~Config();

	/*void resetToDefaults();*/
	void loadFromFile();
	void saveToFile();

	// Defaults
	const unsigned short int _net_v4_udp_port = 2185;
	const std::string _net_v4_udp_bind = "0.0.0.0";
	const int _net_v4_lpd_timer = 10;
	const std::string _net_v4_lpd_address = "239.192.152.144";

	const unsigned short int _net_v6_udp_port = 2185;
	const std::string _net_v6_udp_bind = "0::0";
	const int _net_v6_lpd_timer = 10;
	const std::string _net_v6_lpd_address = "ff08::BD02";

	// Configurations
	unsigned short int net_v4_udp_port;
	std::string net_v4_udp_bind;
	int net_v4_lpd_timer;
	std::string net_v4_lpd_address;

	unsigned short int net_v6_udp_port;
	std::string net_v6_udp_bind;
	int net_v6_lpd_timer;
	std::string net_v6_lpd_address;
};

} /* namespace p2pnet */
#endif /* CONFIG_H_ */
