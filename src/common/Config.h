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
#include <mutex>

namespace p2pnet {

typedef boost::property_tree::ptree config_t;

class ConfigClient {
	friend class ConfigManager;
	ConfigManager& parent_config_manager;
protected:
	ConfigClient(ConfigManager& parent_config) : parent_config_manager(parent_config){
		parent_config_manager.registerClient(this);
	}
	virtual ~ConfigClient(){
		parent_config_manager.registerClient(this);
	}

	virtual void configChanged() = 0;

	inline config_t getConfig(){return parent_config_manager.getConfig();};
	inline void putConfig(config_t config){parent_config_manager.putConfig(config);};
};

class ConfigManager {
	config_t internal_config;
	std::string config_directory;
	std::string config_file;

	std::mutex config_io_mutex;

	/**
	 * Returns full path to configuration file.
	 * @note Platform-dependent. Also, if superuser, it tries to save config system-wide (in /etc).
	 * @return
	 */
	void setDefaultConfigFilepath();

	void resetToDefaults();
	void loadFromFile();
	void saveToFile();
public:
	ConfigManager();
	ConfigManager(std::string config_filepath);
	virtual ~ConfigManager();

	inline config_t getConfig(){
		return internal_config;
	};

	void putConfig(config_t config);

	config_t getDefaults(){
		config_t config;

		config.put("net.sockets.udp.enable", true);
		config.put("net.sockets.udp.local_ip", "0::0");
		config.put("net.sockets.udp.port", 2185);
		config.put("net.lpd.udp.enable", true);
		config.put("net.lpd.udp.timer", 10);
		config.put("net.lpd.udp.local_ip", "0::0");
		config.put("net.lpd.udp.v4.host", "239.192.152.144");
		config.put("net.lpd.udp.v4.port", 28915);
		config.put("net.lpd.udp.v6.host", "ff08::BD02");
		config.put("net.lpd.udp.v6.port", 28915);

		return config;
	};

	std::string getDefaultConfigFilepath();
	void setConfigFilepath(std::string filepath);

	std::set<ConfigClient*> config_clients;
	void registerClient(ConfigClient* client);
	void removeClient(ConfigClient* client);

	void configChanged();
};

} /* namespace p2pnet */
#endif /* CONFIG_H_ */
