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

Config::Config() {
	setDefaultConfigFilepath();
	this->loadFromFile();
}
Config::Config(std::string config_filepath) {

}

Config::~Config() {this->saveToFile();}

std::string Config::getDefaultConfigFilepath() const {
	// Setting up paths on multiple platforms

	// First, check P2PNET_CONFIG environment variable
	char* env_filename = getenv("P2PNET_CONFIG");
	if(env_filename){
		return env_filename;
	}

	std::string filepath;

	#ifdef _WIN32
	// Windows part:
	filepath = getenv("AppData");
	filepath += "\\P2PNet\\";
	#elif defined __unix__
	// Linux, Mac OS X, etc.
	if(getuid() == 0){
		filepath = "/etc/p2pnet/";
	}else{
		filepath = getenv("HOME");
		filepath += "/.p2pnet/";
	}
	#endif

	config_filepath = filepath+"p2pnet.xml";

	return config_filepath;
}

void Config::setConfigFilepath(std::string filepath){
	config_filepath = filepath;
}

void Config::resetToDefaults() {
	pt.clear();
}

void Config::loadFromFile() {
	read_xml(config_filepath, pt);
}

void Config::saveToFile() {
	write_xml(config_filepath, pt);
}

} /* namespace p2pnet */
