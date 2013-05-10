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

#ifndef ABSTRACTNETDBSTORAGE_H_
#define ABSTRACTNETDBSTORAGE_H_

#include "../peer/TH.h"
#include "../protobuf/NetDBEntry.pb.h"
#include <list>

namespace p2pnet {
namespace databases {

class AbstractNetDBStorage {
public:
	AbstractNetDBStorage(){};
	virtual ~AbstractNetDBStorage(){};

	typedef std::list<peer::TH> hashlist_t;
	virtual hashlist_t getAllHashes() = 0;

	virtual bool hasEntry(const peer::TH& peer_th) = 0;
	virtual NetDBEntry& getEntry(const peer::TH& peer_th) = 0;

	virtual bool hasRouteToPeer(const peer::TH& peer_th, const TransportSocketEndpoint_s& peer_tse_s) = 0;
	virtual void bumpRouteToPeer(const peer::TH& peer_th, const TransportSocketEndpoint_s& peer_tse_s) = 0;
};

} /* namespace databases */
} /* namespace p2pnet */
#endif /* ABSTRACTNETDBSTORAGE_H_ */
