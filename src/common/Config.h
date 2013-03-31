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

namespace p2pnet {
class Config {
	boost::property_tree::ptree pt;
	std::string config_directory;
	std::string config_file;

	/**
	 * Returns full path to configuration file.
	 * @note Platform-dependent. Also, if superuser, it tries to save config system-wide.
	 * @return
	 */
	void setDefaultConfigFilepath();
public:
	Config();
	Config(std::string config_filepath);
	virtual ~Config();

	typedef boost::property_tree::ptree config_t;

	config_t& getConfig(){
		return pt;
	};
/*
	config_t getDefaults(){
		config_t config;

		config.net_udp_v4_enable = true;
		config.net_udp_v4_port = 2185;
		config.net_udp_v4_bind = "0.0.0.0";
		config.net_udp_v4_lpd_enable = true;
		config.net_udp_v4_lpd_timer = 10;
		config.net_udp_v4_lpd_address = "239.192.152.144";
		config.net_udp_v4_lpd_port = 28915;
		config.net_udp_v4_lpd_bind = "0.0.0.0";

		config.net_udp_v6_enable = true;
		config.net_udp_v6_port = 2185;
		config.net_udp_v6_bind = "0::0";
		config.net_udp_v6_lpd_enable = true;
		config.net_udp_v6_lpd_timer = 10;
		config.net_udp_v6_lpd_address = "ff08::BD02";
		config.net_udp_v6_lpd_port = 28915;
		config.net_udp_v6_lpd_bind = "0::0";

		return config;
	};
*/
	std::string getDefaultConfigFilepath();
	void setConfigFilepath(std::string filepath);

	void resetToDefaults();
	void loadFromFile();
	void saveToFile();
};

} /* namespace p2pnet */
#endif /* CONFIG_H_ */
