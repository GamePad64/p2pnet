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

template<typename pubkeymap_t, typename routemap_t>
MapNetDBStorage<pubkeymap_t, routemap_t>::MapNetDBStorage() {}
template<typename pubkeymap_t, typename routemap_t>
MapNetDBStorage<pubkeymap_t, routemap_t>::~MapNetDBStorage() {}

template<typename pubkeymap_t, typename routemap_t>
bool MapNetDBStorage<pubkeymap_t, routemap_t>::hasPublicKeyOf(crypto::hash_t peer_id){
	return m_pubkeymap.count(peer_id) != 0;
}

template<typename pubkeymap_t, typename routemap_t>
crypto::key_public_t MapNetDBStorage<pubkeymap_t, routemap_t>::getPublicKeyOf(crypto::hash_t peer_id){
	return this->m_pubkeymap[peer_id];
}
template<typename pubkeymap_t, typename routemap_t>
void MapNetDBStorage<pubkeymap_t, routemap_t>::setPublicKeyOf(crypto::hash_t peer_id, crypto::key_public_t key_public){
	this->m_pubkeymap[peer_id] = key_public;
	this->m_hashes.push_front(peer_id);
}

template<typename pubkeymap_t, typename routemap_t>
bool MapNetDBStorage<pubkeymap_t, routemap_t>::hasRouteTo(crypto::hash_t peer_id){
	return m_routemap.count(peer_id) != 0;
}
template<typename pubkeymap_t, typename routemap_t>
PeerRouteSet MapNetDBStorage<pubkeymap_t, routemap_t>::getRouteTo(crypto::hash_t peer_id){
	return this->m_routemap[peer_id];
}
template<typename pubkeymap_t, typename routemap_t>
void MapNetDBStorage<pubkeymap_t, routemap_t>::setRouteOf(crypto::hash_t peer_id, PeerRouteSet& route){
	this->m_routemap[peer_id] = route;
}

} /* namespace databases */
} /* namespace p2pnet */
