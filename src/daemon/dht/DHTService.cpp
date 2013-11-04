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

DHTService::DHTService() {
	k_buckets.resize(crypto::HASH_LENGTH);
}

DHTService::~DHTService() {}

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

void DHTService::registerListener(DHTListener* listener_ptr, std::string namespace_hook) {
	ns_listeners[namespace_hook].insert(listener_ptr);
}

void DHTService::unregisterListener(DHTListener* listener_ptr, std::string namespace_hook) {
	ns_listeners[namespace_hook].erase(listener_ptr);
}

} /* namespace dht */
} /* namespace p2pnet */
