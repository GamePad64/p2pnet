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
#ifndef DHTSERVICE_H_
#define DHTSERVICE_H_

#include "../../common/crypto/Hash.h"
#include <unordered_map>

namespace p2pnet {
namespace dht {

class DHTClient : boost::noncopyable {
	DHTService* service_ptr;
public:
	DHTClient(const DHTService& parent_service);
	virtual ~DHTClient();
	// Signals
	virtual void foundValue(std::string ns, crypto::Hash hash, std::string value) = 0;

	virtual std::string findValue(crypto::Hash hash) = 0;
	virtual std::string postValue(std::string ns, const crypto::Hash& hash);
};

class DHTStoredValue {
public:
	boost::posix_time::ptime timestamp_expires;
	std::string value;
};

struct DHTCoords {
	std::string ns;	// I can't use word "namespace", it is reserved.
	crypto::Hash hash;
};

/**
 * This is what we call 2D-DHT. It is a modified version of Kademlia protocol.
 * 2D means that we have namespace and keyspace.
 */
class DHTService : boost::noncopyable {
	friend class DHTClient;
private:
	std::multimap<std::string, DHTClient*> dht_clients;

	// Values received from other nodes.
	std::unordered_map<DHTCoords, std::shared_ptr<DHTStoredValue>> stored_values;
	std::deque<std::weak_ptr<DHTStoredValue>> ordered_value_expiration_queue;

	void findNodes(const crypto::Hash& hash);

	virtual boost::optional<std::string> getLocalValue(DHTCoords coords) = 0;
	boost::optional<std::string> getStoredValue(DHTCoords coords);
protected:
	virtual void send(const crypto::Hash& dest, const protocol::DHTPart& dht_part) = 0;
	virtual void process(const crypto::Hash& from, const protocol::DHTPart& dht_part) = 0;

	void findValue(DHTClient* client, DHTCoords coords, const crypto::Hash& hash);
	void postValue(DHTClient* client, DHTCoords coords, std::string value);

	/* Listener mgmt */
	void registerClient(DHTClient* listener_ptr, std::string namespace_hook);
	void unregisterClient(DHTClient* listener_ptr, std::string namespace_hook);
public:
	DHTService();
	virtual ~DHTService();
};

} /* namespace dht */
} /* namespace p2pnet */

#endif /* DHTSERVICE_H_ */
