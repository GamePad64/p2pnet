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
ConfigManager::ConfigManager() : ConfigManager(getDefaultConfigFilepath()) {}

ConfigManager::ConfigManager(std::string config_filepath) {
	this->setConfigFilepath(config_filepath);
	this->loadFromFile();
}

ConfigManager::~ConfigManager() {this->saveToFile();}

std::string ConfigManager::getDefaultConfigFilepath() {
	// Setting up paths on multiple platforms

	// First, check P2PNET_CONFIG environment variable
	char* env_filename = getenv("P2PNET_CONFIG");
	if(env_filename){
		std::string s(env_filename);
		delete[] env_filename;
		return s;
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

void ConfigManager::setConfigFilepath(std::string filepath){
	log() << "Configuration path set: " << filepath << std::endl;
	config_file = filepath;
}

void ConfigManager::putConfig(config_t config) {
	config_io_mutex.lock();	// No RAII, hardcore only!

	internal_config = config;
	saveToFile();	// TODO: This will be created in a separate thread.

	config_io_mutex.unlock();
}

void ConfigManager::resetToDefaults() {
	config_io_mutex.lock();
	internal_config.clear();
	config_io_mutex.unlock();
}

void ConfigManager::loadFromFile() {
	boost::filesystem::create_directory(config_directory);
	config_io_mutex.lock();

	std::ofstream file(config_file);
	file.close();
	read_xml(config_file, internal_config);

	config_io_mutex.unlock();
	log() << "Configuration file loaded" << std::endl;
}

void ConfigManager::saveToFile() {
	boost::filesystem::create_directory(config_directory);
	write_xml(config_file, internal_config);
	log() << "Configuration file saved" << std::endl;
}

void ConfigManager::registerClient(ConfigClient* client) {
	config_clients.insert(client);
}

void ConfigManager::unregisterClient(ConfigClient* client) {
	config_clients.erase(client);
}

void ConfigManager::configChanged() {
	for(auto client : config_clients){
		client->configChanged();
	}
}

} /* namespace p2pnet */
