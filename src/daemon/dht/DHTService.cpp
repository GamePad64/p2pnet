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
#include "DHTService.h"

namespace p2pnet {
namespace dht {

DHTClient::DHTClient(const DHTService& parent_service){
	service_ptr = &parent_service;
}

DHTService::DHTService() {
	k_buckets.resize(crypto::HASH_LENGTH);
}

DHTService::~DHTService() {}

void findValue(std::string ns, const crypto::Hash& hash){

}
void postValue(std::string ns, const crypto::Hash& hash, std::string value){

}

/* K-bucket mgmt */
void DHTService::registerInKBucket(const crypto::Hash& hash, unsigned short distance) {
	k_buckets[distance].insert(hash);
}
void DHTService::registerInKBucket(const crypto::Hash& hash, const crypto::Hash& my_hash) {
	unsigned short distance = my_hash.computeDistance(hash);
	registerInKBucket(hash, distance);
}
void DHTService::removeFromKBucket(const crypto::Hash& hash, unsigned short distance){
	k_buckets[distance].erase(hash);
}
void DHTService::removeFromKBucket(const crypto::Hash& hash, const crypto::Hash& my_hash){
	unsigned short distance = my_hash.computeDistance(hash);
	removeFromKBucket(hash, distance);
}

/* Listener mgmt */
void DHTService::registerClient(DHTClient* listener_ptr, std::string namespace_hook) {
	dht_clients.insert(std::make_pair(namespace_hook, listener_ptr));
}
void DHTService::unregisterClient(DHTClient* listener_ptr, std::string namespace_hook) {
	auto ns_range = dht_clients.equal_range(namespace_hook);
	for(auto it = ns_range.first; it != ns_range.second; ++it){
		if(*it == listener_ptr){
			dht_clients.erase(it);
			break;
		}
	}
}

} /* namespace dht */
} /* namespace p2pnet */
