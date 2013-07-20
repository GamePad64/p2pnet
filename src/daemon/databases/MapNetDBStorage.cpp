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
#include <ctime>
#include <stdint.h>

namespace p2pnet {
namespace databases {

template< typename peermap_t >
MapNetDBStorage< peermap_t >::MapNetDBStorage() {
}
template< typename peermap_t >
MapNetDBStorage< peermap_t >::~MapNetDBStorage() {
}

template< typename peermap_t >
bool MapNetDBStorage< peermap_t >::hasEntry(const peer::TH& peer_th) {
	return m_peermap.count(peer_th.toBinaryVector()) != 0;
}

template< typename peermap_t >
NetDBEntry& MapNetDBStorage< peermap_t >::getEntry(const peer::TH& peer_th) {
	crypto::Hash::binary_vector_t th_v = peer_th.toBinaryVector();

	return m_peermap[th_v];
}

template< typename peermap_t >
bool MapNetDBStorage< peermap_t >::hasRouteToPeer(const peer::TH& peer_th, const TransportSocketEndpoint_s& peer_tse_s){
	NetDBEntry& entry = getEntry(peer_th);
	for(auto& endpoint : entry.endpoints()){
		if(endpoint.SerializeAsString() == peer_tse_s.SerializeAsString())
			return true;
	}
	return false;
}

template< typename peermap_t >
void MapNetDBStorage< peermap_t >::bumpRouteToPeer(const peer::TH& peer_th, const TransportSocketEndpoint_s& peer_tse_s){
	NetDBEntry& entry = getEntry(peer_th);
	for(auto endpoint_i = entry.mutable_endpoints()->begin(); endpoint_i != entry.mutable_endpoints()->end(); endpoint_i++){
		if(endpoint_i->SerializeAsString() == peer_tse_s.SerializeAsString()){
			endpoint_i->set_last_usage((google::protobuf::int64)std::time(0));
			break;
		}
	}
}

template class MapNetDBStorage< std::map< crypto::Hash::binary_vector_t, NetDBEntry > > ;
template class MapNetDBStorage< boost::unordered_map< crypto::Hash::binary_vector_t, NetDBEntry > > ;
template class MapNetDBStorage< btree::safe_btree_map< crypto::Hash::binary_vector_t, NetDBEntry > > ;

} /* namespace databases */
} /* namespace p2pnet */
