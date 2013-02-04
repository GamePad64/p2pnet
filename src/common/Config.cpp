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

	net_v4_udp_port = pt.get("net.v4.udp.port", this->_net_v4_udp_port);
	net_v4_udp_bind = pt.get("net.v4.udp.bind", this->_net_v4_udp_bind);
	net_v4_lpd_timer = pt.get("net.v4.lpd.timer", this->_net_v4_lpd_timer);
	net_v4_lpd_address = pt.get("net.v4.lpd.address", this->_net_v4_lpd_address);

	net_v6_udp_port = pt.get("net.v6.udp.port", this->_net_v6_udp_port);
	net_v6_udp_bind = pt.get("net.v6.udp.bind", this->_net_v6_udp_bind);
	net_v6_lpd_timer = pt.get("net.v6.lpd.timer", this->_net_v6_lpd_timer);
	net_v6_lpd_address = pt.get("net.v6.lpd.address", this->_net_v6_lpd_address);
}

void Config::saveToFile() {
	pt.put("net.v4.udp.port", net_v4_udp_port);
	pt.put("net.v4.udp.bind", net_v4_udp_bind);
	pt.put("net.v4.lpd.timer", net_v4_lpd_timer);
	pt.put("net.v4.lpd.address", net_v4_lpd_address);

	pt.put("net.v6.udp.port", net_v6_udp_port);
	pt.put("net.v6.udp.bind", net_v6_udp_bind);
	pt.put("net.v6.lpd.timer", net_v6_lpd_timer);
	pt.put("net.v6.lpd.address", net_v6_lpd_address);

	write_xml(getConfigFilename(), pt);
}

} /* namespace p2pnet */
