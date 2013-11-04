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

namespace p2pnet {
namespace dht {

class DHTListener {
	// Signals
	void DHT_foundValue(std::string ns, crypto::Hash hash, std::string value);
};

/**
 * This is what we call 2D-DHT. It is a modified version of Kademlia protocol.
 * 2D means that we have namespace and keyspace.
 */
class DHTService {
private:
	std::map<std::string, std::set<DHTListener*>> ns_listeners;
	std::vector<std::set<crypto::Hash>> k_buckets;

	void findNodes(const crypto::Hash& hash);
protected:
	virtual void send(const crypto::Hash& dest, const protocol::DHTPart& dht_part);
	virtual void process(const crypto::Hash& from, const protocol::DHTPart& dht_part);
public:
	DHTService();
	virtual ~DHTService();

	void findValue(std::string ns, const crypto::Hash& hash);
	void postValue(std::string ns, const crypto::Hash& hash, std::string value);

	virtual void registerInKBucket(const crypto::Hash& hash, unsigned short distance);
	void registerInKBucket(const crypto::Hash& hash, const crypto::Hash& my_hash);

	virtual void removeFromKBucket(const crypto::Hash& hash, unsigned short distance);
	void removeFromKBucket(const crypto::Hash& hash, const crypto::Hash& my_hash);

	void registerListener(DHTListener* listener_ptr, std::string namespace_hook);
	void unregisterListener(DHTListener* listener_ptr, std::string namespace_hook);
};

} /* namespace dht */
} /* namespace p2pnet */

#endif /* DHTSERVICE_H_ */
