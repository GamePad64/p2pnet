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

#include <boost/filesystem.hpp>

namespace p2pnet {

Config::Config() {
	this->setDefaultConfigFilepath();
	this->loadFromFile();
}

Config::Config(std::string config_filepath) {
	this->setConfigFilepath(config_filepath);
	this->loadFromFile();
}

Config::~Config() {this->saveToFile();}

std::string Config::getDefaultConfigFilepath() {
	// Setting up paths on multiple platforms

	// First, check P2PNET_CONFIG environment variable
	char* env_filename = getenv("P2PNET_CONFIG");
	if(env_filename){
		return env_filename;
	}

	#ifdef _WIN32
	// Windows part:
	config_directory = getenv("AppData");
	config_directory += "\\P2PNet\\";
	#elif defined __unix__
	// Linux, Mac OS X, etc.
	if(getuid() == 0){
		config_directory = "/etc/p2pnet/";
	}else{
		config_directory = getenv("HOME");
		config_directory += "/.p2pnet/";
	}
	#endif

	config_file = config_directory+"p2pnet.xml";

	return config_file;
}

void Config::setDefaultConfigFilepath() {
	this->setConfigFilepath(this->getDefaultConfigFilepath());
}

void Config::setConfigFilepath(std::string filepath){
	config_file = filepath;
}

void Config::resetToDefaults() {
	pt.clear();
}

void Config::loadFromFile() {
	boost::filesystem::create_directory(config_directory);
	std::ofstream file(config_file);
	file.close();
	read_xml(config_file, pt);
}

void Config::saveToFile() {
	write_xml(config_file, pt);
}

} /* namespace p2pnet */
