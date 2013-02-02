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

MapNetDBStorage::MapNetDBStorage() {}
MapNetDBStorage::~MapNetDBStorage() {}

bool MapNetDBStorage::hasPublicKeyOf(crypto::hash_t peer_id){
	return m_pubkeymap.count(peer_id) != 0;
}
crypto::key_public_t MapNetDBStorage::getPublicKeyOf(crypto::hash_t peer_id){
	return this->m_pubkeymap[peer_id];
}
void MapNetDBStorage::setPublicKeyOf(crypto::hash_t peer_id, crypto::key_public_t key_public){
	this->m_pubkeymap[peer_id] = key_public;
	this->m_hashes.push_front(peer_id);
}

bool MapNetDBStorage::hasRouteTo(crypto::hash_t peer_id){
	return m_routemap.count(peer_id) != 0;
}
PeerRouteSet MapNetDBStorage::getRouteTo(crypto::hash_t peer_id){
	return this->m_routemap[peer_id];
}
void MapNetDBStorage::setRouteOf(crypto::hash_t peer_id, PeerRouteSet& route){
	this->m_routemap[peer_id] = route;
}

} /* namespace databases */
} /* namespace p2pnet */
