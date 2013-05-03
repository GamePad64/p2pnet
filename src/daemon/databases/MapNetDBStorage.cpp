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

template <typename peermap_t>
MapNetDBStorage<peermap_t>::MapNetDBStorage() {
}
template <typename peermap_t>
MapNetDBStorage<peermap_t>::~MapNetDBStorage() {
}

template <typename peermap_t>
bool MapNetDBStorage<peermap_t>::hasPeer(const peer::TH& peer_th) {
	return m_peermap.count(peer_th.toBinaryVector()) != 0;
}

template <typename peermap_t>
peer::Peer& MapNetDBStorage<peermap_t>::getPeer(peer::TH& peer_th) {
	crypto::Hash::binary_vector_t th_v = peer_th.toBinaryVector();

	if(hasPeer(peer_th)){
		return *(m_peermap[th_v]);
	}

	m_peermap[th_v] = peer::Peer::pointer(new peer::Peer(peer_th));

	return *(m_peermap[th_v]);
}

template class MapNetDBStorage<std::map<crypto::Hash::binary_vector_t, peer::Peer::pointer> > ;
template class MapNetDBStorage< btree::safe_btree_map< crypto::Hash::binary_vector_t, peer::Peer::pointer > > ;

} /* namespace databases */
} /* namespace p2pnet */
