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
#ifndef OVERLAYDHT_H_
#define OVERLAYDHT_H_

#include "../dht/DHTService.h"

namespace p2pnet {
namespace overlay {

class OverlaySocket;

class OverlayDHT : public dht::DHTService {
	OverlaySocket* parent_socket_ptr;
	std::array<std::set<std::shared_ptr<OverlayPeer>>, crypto::HASH_LENGTH> k_buckets;
protected:
	virtual void send(const crypto::Hash& dest, const protocol::DHTPart& dht_part);
	virtual void process(const crypto::Hash& from, const protocol::DHTPart& dht_part);
public:
	OverlayDHT();
	OverlayDHT(OverlaySocket* socket_ptr);
	virtual ~OverlayDHT();

	void registerInKBucket(std::shared_ptr<OverlayPeer> peer, unsigned short distance);
	void registerInKBucket(std::shared_ptr<OverlayPeer> peer, const crypto::Hash& my_hash);
	void removeFromKBucket(std::shared_ptr<OverlayPeer> peer, unsigned short distance);
	void removeFromKBucket(std::shared_ptr<OverlayPeer> peer, const crypto::Hash& my_hash);
};

} /* namespace overlay */
} /* namespace p2pnet */

#endif /* OVERLAYDHT_H_ */
