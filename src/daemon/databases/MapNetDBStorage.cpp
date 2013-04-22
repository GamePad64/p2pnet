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

#include "MapNetDBStorage.h"

namespace p2pnet {
namespace databases {

template<typename peermap_t>
MapNetDBStorage<peermap_t>::MapNetDBStorage() {}
template<typename peermap_t>
MapNetDBStorage<peermap_t>::~MapNetDBStorage() {}

template<typename peermap_t>
bool MapNetDBStorage<peermap_t>::hasPeer(crypto::Hash peer_id){
	return m_peermap.count(peer_id.toBinaryVector()) > 0;
}

template<typename peermap_t>
peer::Peer& MapNetDBStorage<peermap_t>::getPeer(crypto::Hash peer_th){
	return this->m_peermap[peer_th.toBinaryVector()];
}

template class MapNetDBStorage< std::map<crypto::Hash::binary_vector_t, peer::Peer> >;
template class MapNetDBStorage< btree::safe_btree_map<crypto::Hash::binary_vector_t, peer::Peer> >;

} /* namespace databases */
} /* namespace p2pnet */
