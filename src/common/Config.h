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
#include <set>

namespace p2pnet {

typedef boost::property_tree::ptree config_t;

class ConfigManager;

class ConfigClient {
	friend class ConfigManager;
	ConfigManager& parent_config_manager;
protected:
	ConfigClient(ConfigManager& parent_config);
	virtual ~ConfigClient();

	virtual void configChanged() = 0;

	config_t getConfig();
	void putConfig(config_t config);

	config_t getDefaults();

	template<class T>
	T getConfigValueOrDefault(std::string path);
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

		config.put("net.lpd.udpv4.enable", true);
		config.put("net.lpd.udpv4.timer", 10);
		config.put("net.lpd.udpv4.local_ip", "0.0.0.0");
		config.put("net.lpd.udpv4.mcast.host", "239.192.152.144");
		config.put("net.lpd.udpv4.mcast.port", 28915);

		config.put("net.lpd.udpv6.enable", true);
		config.put("net.lpd.udpv6.timer", 10);
		config.put("net.lpd.udpv6.local_ip", "0::0");
		config.put("net.lpd.udpv6.mcast.host", "ff08::BD02");
		config.put("net.lpd.udpv6.mcast.port", 28915);

		return config;
	};

	std::string getDefaultConfigFilepath();
	void setConfigFilepath(std::string filepath);

	std::set<ConfigClient*> config_clients;
	void registerClient(ConfigClient* client);
	void removeClient(ConfigClient* client);

	void configChanged();

	template<class T>
	T getConfigValueOrDefault(std::string path);
};

template< class T >
inline T ConfigClient::getConfigValueOrDefault(std::string path){
	return parent_config_manager.getConfigValueOrDefault<T>(path);
}

template< class T >
inline T ConfigManager::getConfigValueOrDefault(std::string path) {
	return getConfig().get(path, getDefaults().get<T>(path));
}

} /* namespace p2pnet */

#endif /* CONFIG_H_ */
