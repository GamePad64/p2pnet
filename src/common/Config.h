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

#include "Loggable.h"
#include "Singleton.h"
#include <boost/property_tree/ptree.hpp>
#include <mutex>
#include <set>

namespace p2pnet {

typedef boost::property_tree::ptree config_t;

class ConfigManager;

class ConfigClient {
	friend class ConfigManager;
protected:
	ConfigClient();
	virtual ~ConfigClient();

	virtual void configChanged(){};	// It is permitted not to handle this, but it is not good though.

	template<class T>
	T getValue(std::string path);
	template<class T>
	void setValue(std::string path, T value);
};

class ConfigManager : public Loggable, public Singleton<ConfigManager> {
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

		config.put("transport.udp.enabled", true);
		config.put("transport.udp.local_ip", "0::0");
		config.put("transport.udp.port", 2185);

		config.put("databases.pks.history_size", 10);
		config.put("databases.pks.renew_interval", 10);

		config.put("discovery.udpv4.enabled", true);
		config.put("discovery.udpv4.timer", 10);
		config.put("discovery.udpv4.local_ip", "0.0.0.0");
		config.put("discovery.udpv4.multicast.ip", "239.192.152.144");
		config.put("discovery.udpv4.multicast.port", 28915);

		config.put("discovery.udpv6.enabled", true);
		config.put("discovery.udpv6.timer", 10);
		config.put("discovery.udpv6.local_ip", "0::0");
		config.put("discovery.udpv6.multicast.ip", "ff08::BD02");
		config.put("discovery.udpv6.multicast.port", 28915);

		return config;
	};

	std::string getDefaultConfigFilepath();
	void setConfigFilepath(std::string filepath);

	std::set<ConfigClient*> config_clients;
	void registerClient(ConfigClient* client);
	void unregisterClient(ConfigClient* client);

	void configChanged();

	template<class T>
	T getValue(std::string path);
	template<class T>
	void setValue(std::string path, T value);
};

template< class T >
inline T ConfigClient::getValue(std::string path){
	return ConfigManager::getInstance()->getValue<T>(path);
}

template< class T >
inline void ConfigClient::setValue(std::string path, T value){
	return ConfigManager::getInstance()->setValue(path, value);
}

template< class T >
inline T ConfigManager::getValue(std::string path) {
	return getConfig().get(path, getDefaults().get<T>(path));
}

template< class T >
inline void ConfigManager::setValue(std::string path, T value) {
	return getConfig().put(path, value);
}

} /* namespace p2pnet */

#endif /* CONFIG_H_ */
