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

#include "Config.h"

#ifdef __unix__
#include <unistd.h>
#elif defined _WIN32
#include <windows.h>
#endif

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/info_parser.hpp>

namespace p2pnet {

Config::Config() {this->loadFromFile();}
Config::~Config() {this->saveToFile();}

const std::string Config::getConfigFilename(bool force) const {
	// Setting up paths on multiple platforms
	if(!this->config_filename.empty() && !force){
		return config_filename;
	}

	char* env_filename = getenv("P2PNET_CONFIG");
	if(env_filename){
		config_filename = env_filename;
		return env_filename;
	}

	std::string filepath;
	//TODO: Apply system-wide configuration if run on Windows by administrator.
	#ifdef _WIN32
	filepath = getenv("AppData");
	filepath += "\\P2PNet\\";
	#elif defined __unix__
	// If running on root, then use /etc
	if(getuid() == 0){
		filepath = "/etc/p2pnet/";
	}else{
		filepath = getenv("HOME");
		filepath += "/.p2pnet/";
	}
	#endif

	config_filename = filepath+"p2pnet.xml";

	return config_filename;
}

/*void Config::resetToDefaults() {}*/

void Config::loadFromFile() {
	read_xml(getConfigFilename(), pt);

	this_config.net_udp_v4_enable = pt.get("net.udp.v4.enable", this->getDefaults().net_udp_v4_enable);
	this_config.net_udp_v4_port = pt.get("net.udp.v4.port", this->getDefaults().net_udp_v4_port);
	this_config.net_udp_v4_bind = pt.get("net.udp.v4.bind", this->getDefaults().net_udp_v4_bind);
	this_config.net_udp_v4_lpd_enable = pt.get("net.udp.v4.lpd.enable", this->getDefaults().net_udp_v4_lpd_enable);
	this_config.net_udp_v4_lpd_timer = pt.get("net.udp.v4.lpd.timer", this->getDefaults().net_udp_v4_lpd_timer);
	this_config.net_udp_v4_lpd_address = pt.get("net.udp.v4.lpd.address", this->getDefaults().net_udp_v4_lpd_address);
	this_config.net_udp_v4_lpd_port = pt.get("net.udp.v4.lpd.port", this->getDefaults().net_udp_v4_lpd_port);
	this_config.net_udp_v4_lpd_bind = pt.get("net.udp.v4.lpd.bind", this->getDefaults().net_udp_v4_lpd_bind);

	this_config.net_udp_v6_enable = pt.get("net.udp.v6.enable", this->getDefaults().net_udp_v6_enable);
	this_config.net_udp_v6_port = pt.get("net.udp.v6.port", this->getDefaults().net_udp_v6_port);
	this_config.net_udp_v6_bind = pt.get("net.udp.v6.bind", this->getDefaults().net_udp_v6_bind);
	this_config.net_udp_v6_lpd_enable = pt.get("net.udp.v6.lpd.enable", this->getDefaults().net_udp_v6_lpd_enable);
	this_config.net_udp_v6_lpd_timer = pt.get("net.udp.v6.lpd.timer", this->getDefaults().net_udp_v6_lpd_timer);
	this_config.net_udp_v6_lpd_address = pt.get("net.udp.v6.lpd.address", this->getDefaults().net_udp_v6_lpd_address);
	this_config.net_udp_v6_lpd_port = pt.get("net.udp.v6.lpd.port", this->getDefaults().net_udp_v6_lpd_port);
	this_config.net_udp_v6_lpd_bind = pt.get("net.udp.v6.lpd.bind", this->getDefaults().net_udp_v6_lpd_bind);
}

void Config::saveToFile() {
	pt.put("net.udp.v4.enable", this->getConfig().net_udp_v4_enable);
	pt.put("net.udp.v4.port", this->getConfig().net_udp_v4_port);
	pt.put("net.udp.v4.bind", this->getConfig().net_udp_v4_bind);
	pt.put("net.udp.v4.lpd.enable", this->getConfig().net_udp_v4_lpd_enable);
	pt.put("net.udp.v4.lpd.timer", this->getConfig().net_udp_v4_lpd_timer);
	pt.put("net.udp.v4.lpd.address", this->getConfig().net_udp_v4_lpd_address);
	pt.put("net.udp.v4.lpd.port", this->getConfig().net_udp_v4_lpd_port);
	pt.put("net.udp.v4.lpd.bind", this->getConfig().net_udp_v4_lpd_bind);

	pt.put("net.udp.v6.enable", this->getConfig().net_udp_v6_enable);
	pt.put("net.udp.v6.port", this->getConfig().net_udp_v6_port);
	pt.put("net.udp.v6.bind", this->getConfig().net_udp_v6_bind);
	pt.put("net.udp.v6.lpd.enable", this->getConfig().net_udp_v6_lpd_enable);
	pt.put("net.udp.v6.lpd.timer", this->getConfig().net_udp_v6_lpd_timer);
	pt.put("net.udp.v6.lpd.address", this->getConfig().net_udp_v6_lpd_address);
	pt.put("net.udp.v6.lpd.port", this->getConfig().net_udp_v6_lpd_port);
	pt.put("net.udp.v6.lpd.bind", this->getConfig().net_udp_v6_lpd_bind);

	write_xml(getConfigFilename(), pt);
}

} /* namespace p2pnet */
