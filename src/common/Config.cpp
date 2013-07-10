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
ConfigClient::ConfigClient(ConfigManager& parent_config) : parent_config_manager(parent_config){
	parent_config_manager.registerClient(this);
}

ConfigClient::~ConfigClient() {
	parent_config_manager.registerClient(this);
}

config_t ConfigClient::getConfig(){
	return parent_config_manager.getConfig();
};

void ConfigClient::putConfig(config_t config){
	parent_config_manager.putConfig(config);
};

// ConfigManager
ConfigManager::ConfigManager() {
	this->setDefaultConfigFilepath();
	this->loadFromFile();
}

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

void ConfigManager::setDefaultConfigFilepath() {
	this->setConfigFilepath(this->getDefaultConfigFilepath());
}

void ConfigManager::setConfigFilepath(std::string filepath){
	config_file = filepath;
}

void ConfigManager::putConfig(config_t config) {
	config_io_mutex.lock();	// No RAII, only hardcore!

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
}

void ConfigManager::saveToFile() {
	write_xml(config_file, internal_config);
}

void ConfigManager::registerClient(ConfigClient* client) {
	config_clients.insert(client);
}

void ConfigManager::removeClient(ConfigClient* client) {
	config_clients.erase(client);
}

void ConfigManager::configChanged() {
	for(auto &client : config_clients){
		client->configChanged();
	}
}

} /* namespace p2pnet */
