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

#define MAX_NODES_TOTAL 30
#define MAX_FROM_BUCKET 5

#include "../../common/crypto/Hash.h"
#include "../protobuf/DHT.pb.h"
#include <unordered_map>
#include <list>

namespace p2pnet {
namespace dht {

struct DHTCoords {
	std::string ns;	// I can't use word "namespace", it is reserved.
	crypto::Hash hash;
};

class DHTService;

class DHTClient : boost::noncopyable {
	DHTService* service_ptr;
public:
	DHTClient(DHTService* parent_service);
	virtual ~DHTClient();
	// Signals
	void findNode(const crypto::Hash& coords);
	virtual void foundNode(const crypto::Hash& coords, std::string node_info) = 0;

	//void findValue(DHTCoords coords);
	//virtual std::string postValue(DHTCoords coords, std::string value);
	//virtual void foundValue(DHTCoords coords, std::string value) = 0;
};

struct DHTStoredValue {
	boost::posix_time::ptime timestamp_expires;
	DHTClient* client_ptr;
	std::string value;
};

/**
 * This is what we call 2D-DHT. It is a modified version of Kademlia protocol.
 * 2D means that we have namespace and keyspace.
 */
class DHTService : boost::noncopyable {
	friend class DHTClient;
private:
	std::set<DHTClient*> clients;	// All the clients hooked to this DHTService
	/* Structures that map queries to clients to inform these clients later */
	std::multimap<crypto::Hash, DHTClient*> node_queries;
	std::multimap<DHTCoords, DHTClient*> value_queries;

	std::map<DHTCoords, DHTStoredValue> posted_values;

	std::list<crypto::Hash> getClosestTo(const crypto::Hash& hash,
			unsigned short max_from_bucket,
			unsigned short max_nodes_total);

	void findNode(const crypto::Hash& hash, DHTClient* client = nullptr);
	void foundNode(const crypto::Hash& hash, std::string node_info);
protected:
	virtual void send(const crypto::Hash& dest, const protocol::DHTPart& dht_part) = 0;
	void process(const crypto::Hash& from, const protocol::DHTPart& dht_part);

	virtual crypto::Hash getMyHash() = 0;
	virtual std::vector<crypto::Hash> getNNodesFromBucket(unsigned short bucket) = 0;
	virtual boost::optional<std::string> getLocalNodeInfo(const crypto::Hash& hash) = 0;
	virtual void putLocalNodeInfo(const crypto::Hash& hash, std::string node_info) = 0;

	/* Client management */
	void registerClient(DHTClient* client_ptr);
	void unregisterClient(DHTClient* client_ptr);
public:
	DHTService();
	virtual ~DHTService();
};

} /* namespace dht */
} /* namespace p2pnet */

#endif /* DHTSERVICE_H_ */
