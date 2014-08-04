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
#include "../../common/Loggable.h"
#include <boost/signals2.hpp>

namespace p2pnet {
namespace overlay {

class OverlaySocket;
class OverlayNode;

class OverlayDHT : public dht::DHTService, Loggable {
	OverlaySocket* parent_socket_ptr;

	boost::signals2::connection key_renewal;
public:
	OverlayDHT(OverlaySocket* socket_ptr);
	virtual ~OverlayDHT();

	void send(const crypto::Hash& dest, const protocol::DHTPart& dht_part);

	crypto::Hash getMyHash();

	void registerInKBucket(OverlayNode* node);
	void removeFromKBucket(OverlayNode* node);

	void rebuild();

	void foundNode(std::string serialized_contact);
};

} /* namespace overlay */
} /* namespace p2pnet */

#endif /* OVERLAYDHT_H_ */
