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
template < typename peermap_t >
class MapNetDBStorage : public NetDBStorage {

private:
	peermap_t m_peermap;
	hashlist_t m_hashes;

public:
	MapNetDBStorage();
	virtual ~MapNetDBStorage();

	hashlist_t getAllHashes(){return m_hashes;};

	// Routing database
	bool hasPeer(crypto::Hash peer_id);
	peer::Peer& getPeer(crypto::Hash peer_id);
};

typedef MapNetDBStorage< std::map<crypto::Hash::binary_vector_t, peer::Peer> > StdMapNetDBStorage;
typedef MapNetDBStorage< btree::safe_btree_map<crypto::Hash::binary_vector_t, peer::Peer> > BMapNetDBStorage;

} /* namespace databases */
} /* namespace p2pnet */

#endif /* MAPNETDBSTORAGE_H_ */
