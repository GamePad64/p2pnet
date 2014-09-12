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
#include <sys/types.h>
#include <pwd.h>
#elif defined _WIN32
#include <windows.h>
#endif

#include <boost/property_tree/json_parser.hpp>
//#include <boost/property_tree/xml_parser.hpp>
//#include <boost/property_tree/ini_parser.hpp>
//#include <boost/property_tree/info_parser.hpp>

#include <boost/filesystem.hpp>

namespace p2pnet {

// ConfigManager
ConfigManager::ConfigManager() {
	initDefaults();
	config_directory = getDefaultDirectory();
	config_file = getDefaultFile();

	loadFromFile();
	write_json(config_directory+"defaults.json", getDefaults());
}

ConfigManager::~ConfigManager() {}

const config_t& ConfigManager::getDefaults() const {
	return internal_config_defaults;
}

std::string ConfigManager::getDefaultDirectory() {
	// Setting up paths on multiple platforms

	// First, check P2PNET_CONFIG environment variable
	char* env_config_directory = getenv("P2PNET_CONFIG_DIR");
	if(env_config_directory){
		std::string s(env_config_directory);
		return s;
	}

	std::string ret_config_directory;

	#ifdef _WIN32	// Windows part:

	ret_config_directory = getenv("APPDATA");
	ret_config_directory += "\\P2PNet\\";

	#elif defined __unix__	// Linux. OS X and other Unix systems may work too [not tested, I have no Mac :-(].

	if(getPermissions() == SYSTEM){
		ret_config_directory = "/etc/p2pnet/";
	}else{
		char* env_home_directory = getenv("HOME");
		if(env_home_directory){
			ret_config_directory = env_home_directory;
		}else{	// HOME is empty or inaccessible.
			passwd *pw = getpwuid(getuid());
			ret_config_directory = pw->pw_dir;
		}
		ret_config_directory += "/.p2pnet/";
	}

	#endif

	return ret_config_directory;
}

void ConfigManager::setDirectory(std::string directory){
	log() << "Configuration directory set: " << directory << std::endl;
	config_directory = directory;
}

std::string ConfigManager::getDefaultFile(){
	return "p2pnet.json";
}

void ConfigManager::setFile(std::string filename){
	log() << "Configuration file set: " << filename << std::endl;
	config_file = filename;
}

void ConfigManager::resetToDefaults() {
	config_io_mutex.lock();
	internal_config.clear();
	config_io_mutex.unlock();
}

void ConfigManager::loadFromFile() {
	boost::filesystem::create_directories(config_directory);
	config_io_mutex.lock();

	std::fstream file(config_directory+config_file);
	try {
		read_json(file, internal_config);
		log() << "Configuration file loaded" << std::endl;
	}catch(boost::property_tree::file_parser_error& parser_error){
		internal_config = internal_config_defaults;
		log() << "Configuration file not found. Using default values." << std::endl;
	}
	file.close();

	config_io_mutex.unlock();
}

std::string ConfigManager::getDirectory(){
	return config_directory;
}
std::string ConfigManager::getFile(){
	return config_file;
}

ConfigManager::permissions_t ConfigManager::getPermissions(){
#ifdef _WIN32	// Windows. TODO: Dunno, always USER for now.
	return USER;
#elif defined __unix__	// Linux, we just check for root.
	return (getuid() == 0) ? SYSTEM : USER;
#endif
}

} /* namespace p2pnet */
