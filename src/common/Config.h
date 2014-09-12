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
protected:
	ConfigClient(){};
	virtual ~ConfigClient(){};

	template<class T>
	T getValue(std::string path) const;
};

class ConfigManager : public Loggable, public Singleton<ConfigManager> {
	config_t internal_config;
	config_t internal_config_defaults;
	std::string config_directory;
	std::string config_file;

	std::mutex config_io_mutex;

	void initDefaults() {
		config_t& config = internal_config_defaults;

		// Policies
		config.put("policies.outgoing_only", false);	// TODO:Drops all incoming transport::Connection, disables UDPLPD and ignores ConnectionRequests.

		// API
		// -- Unix Domain Sockets API
		config.put("api.unix.enabled", true);
		config.put("api.unix.user_sock_name", "unix_api.sock");
		config.put("api.unix.disable_user_sock", false);	// Sort of security feature, doesn't allow to deduce ~/.p2pnet/
		config.put("api.unix.system_sock_path", "/var/run/p2pnet/unix_api.sock");

		// Transport
		// -- UDP
		config.put("transport.udp.enabled", true);
		config.put("transport.udp.local_ip", "0::0");
		config.put("transport.udp.port", 2185);
		config.put("transport.udp.mtu", 1438);
		config.put("transport.udp.keepalive_timer", 3);
		config.put("transport.udp.inactivity_timeout", 30);
		config.put("transport.udp.outgoing_only", false);

		// Overlay
		config.put("overlay.connection.processed_queue_size", 100);
		config.put("overlay.connection.key_rotation_spam_limit", 5);
		config.put("overlay.key_provider.history_size", 10);
		config.put("overlay.key_provider.renew_interval", 600);

		// Endpoint
		config.put("p2p.allow_loopback", true);
		config.put("p2p.listen.max_connections", 128);

		// Discovery
		// -- Bootstrap
		config.put("discovery.bootstrap.filename", "bootstrap.txt");

		// -- UDPv4
		config.put("discovery.udpv4.enabled", true);
		config.put("discovery.udpv4.timer", 10);
		config.put("discovery.udpv4.local_ip", "0.0.0.0");
		config.put("discovery.udpv4.multicast.ip", "239.192.152.144");
		config.put("discovery.udpv4.multicast.port", 28915);

		// -- UDPv6
		config.put("discovery.udpv6.enabled", true);
		config.put("discovery.udpv6.timer", 10);
		config.put("discovery.udpv6.local_ip", "0::0");
		config.put("discovery.udpv6.multicast.ip", "ff08::BD02");
		config.put("discovery.udpv6.multicast.port", 28915);
	};
	const config_t& getDefaults() const;

	std::string getDefaultDirectory();
	void setDirectory(std::string directory);

	std::string getDefaultFile();
	void setFile(std::string filename);

	void configChanged();

	void resetToDefaults();
	void loadFromFile();
public:
	ConfigManager();
	virtual ~ConfigManager();

	std::set<ConfigClient*> config_clients;
	void registerClient(ConfigClient* client);
	void unregisterClient(ConfigClient* client);

	std::string getDirectory();
	std::string getFile();

	enum permissions_t {
		SYSTEM,
		USER
	};
	permissions_t getPermissions();

	template<class T>
	T getValue(std::string path) const;
};

template< class T >
inline T ConfigClient::getValue(std::string path) const {
	return ConfigManager::getInstance()->getValue<T>(path);
}

template< class T >
inline T ConfigManager::getValue(std::string path) const {
	try {
		return internal_config.get<T>(path);
	}catch(boost::property_tree::ptree_bad_path&){
		return getDefaults().get<T>(path);
	}
}

} /* namespace p2pnet */

#endif /* CONFIG_H_ */
