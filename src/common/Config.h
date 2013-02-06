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
	const bool _net_udp_v4_enable = true;
	const unsigned short int _net_udp_v4_port = 2185;
	const std::string _net_udp_v4_bind = "0.0.0.0";
	const bool _net_udp_v4_lpd_enable = true;
	const int _net_udp_v4_lpd_timer = 10;
	const std::string _net_udp_v4_lpd_address = "239.192.152.144";
	const unsigned short int _net_udp_v4_lpd_port = 28915;
	const std::string _net_udp_v4_lpd_bind = "0.0.0.0";

	const bool _net_udp_v6_enable = true;
	const unsigned short int _net_udp_v6_port = 2185;
	const std::string _net_udp_v6_bind = "0::0";
	const bool _net_udp_v6_lpd_enable = true;
	const int _net_udp_v6_lpd_timer = 10;
	const std::string _net_udp_v6_lpd_address = "ff08::BD02";
	const unsigned short int _net_udp_v6_lpd_port = 28915;
	const std::string _net_udp_v6_lpd_bind = "0::0";

	// Configurations
	bool net_udp_v4_enable;
	unsigned short int net_udp_v4_port;
	std::string net_udp_v4_bind;
	bool net_udp_v4_lpd_enable;
	int net_udp_v4_lpd_timer;
	std::string net_udp_v4_lpd_address;
	unsigned short int net_udp_v4_lpd_port;
	std::string net_udp_v4_lpd_bind;

	bool net_udp_v6_enable;
	unsigned short int net_udp_v6_port;
	std::string net_udp_v6_bind;
	bool net_udp_v6_enable;
	int net_udp_v6_lpd_timer;
	std::string net_udp_v6_lpd_address;
	unsigned short int net_udp_v6_lpd_port;
	std::string net_udp_v6_lpd_bind;
};

} /* namespace p2pnet */
#endif /* CONFIG_H_ */
