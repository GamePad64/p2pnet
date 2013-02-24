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

#ifndef MAPNETDBSTORAGE_H_
#define MAPNETDBSTORAGE_H_

#include "PeerRouteSet.h"
#include <map>
#include <forward_list>
#include <cpp-btree/safe_btree_map.h>
#include "NetDBStorage.h"

namespace p2pnet {
namespace databases {

/**
 * MapNetDBStorage is a storage for NetDB based on std::map. It has lots of disadvantages:
 *  - It is SLOW.
 *  - It consumes a large amount of memory
 *  - It is stored only in RAM
 * It will be replaced with SQLite database soon.
 *
 * This map version consists of two maps.
 */
template < typename pubkeymap_t, typename routemap_t >
class MapNetDBStorage : public NetDBStorage {

private:
	pubkeymap_t m_pubkeymap;
	routemap_t m_routemap;
	hashlist_t m_hashes;

public:
	MapNetDBStorage();
	virtual ~MapNetDBStorage();

	hashlist_t getAllHashes(){return m_hashes;};

	// Public key database
	bool hasPublicKeyOf(crypto::hash_t peer_id);
	crypto::key_public_t getPublicKeyOf(crypto::hash_t peer_id);
	void setPublicKeyOf(crypto::hash_t peer_id, crypto::key_public_t key_public);

	// Routing database
	bool hasRouteTo(crypto::hash_t peer_id);
	PeerRouteSet getRouteTo(crypto::hash_t peer_id);
	void setRouteOf(crypto::hash_t peer_id, PeerRouteSet& route);
};

typedef MapNetDBStorage< std::map<crypto::hash_t, crypto::key_public_t>, std::map<crypto::hash_t, PeerRouteSet> > StdMapNetDBStorage;
typedef MapNetDBStorage< btree::safe_btree_map<crypto::hash_t, crypto::key_public_t>, btree::safe_btree_map<crypto::hash_t, PeerRouteSet> > BMapNetDBStorage;

} /* namespace databases */
} /* namespace p2pnet */

#endif /* MAPNETDBSTORAGE_H_ */
