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

#include <boost/property_tree/xml_parser.hpp>
//#include <boost/property_tree/json_parser.hpp>
//#include <boost/property_tree/ini_parser.hpp>
//#include <boost/property_tree/info_parser.hpp>

#include <boost/filesystem.hpp>

namespace p2pnet {

// ConfigClient
ConfigClient::ConfigClient() {
	ConfigManager::getInstance()->registerClient(this);
}

ConfigClient::~ConfigClient() {
	ConfigManager::getInstance()->unregisterClient(this);
}

// ConfigManager
ConfigManager::ConfigManager() {
	config_directory = getDefaultDirectory();
	config_file = getDefaultFile();

	loadFromFile();
	write_xml(config_directory+"defaults.xml", getDefaults());
}

ConfigManager::~ConfigManager() {this->saveToFile();}

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

	#elif defined __unix__	// Linux. Mac OS X and other Unix systems are not supported now (but they will be).

	if(getuid() == 0){
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
	return "p2pnet.xml";
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
		read_xml(file, internal_config);
	}catch(boost::property_tree::file_parser_error& parser_error){
		log() << "Configuration file not found. Using default values." << std::endl;
	}
	file.close();

	config_io_mutex.unlock();
	log() << "Configuration file loaded" << std::endl;
}

void ConfigManager::saveToFile() {
	boost::filesystem::create_directories(config_directory);
	write_xml(config_directory+config_file, internal_config);
	log() << "Configuration file saved" << std::endl;
}

void ConfigManager::registerClient(ConfigClient* client) {
	config_clients.insert(client);
}

void ConfigManager::unregisterClient(ConfigClient* client) {
	config_clients.erase(client);
}

std::string ConfigManager::getDirectory(){
	return config_directory;
}
std::string ConfigManager::getFile(){
	return config_file;
}

void ConfigManager::configChanged() {
	for(auto client : config_clients){
		client->configChanged();
	}
}

} /* namespace p2pnet */
