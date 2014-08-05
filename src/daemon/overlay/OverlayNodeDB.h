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
#ifndef OVERLAYNODEDB_H_
#define OVERLAYNODEDB_H_

#include "OverlayNode.h"
#include "../../common/crypto/PublicKeyDSA.h"

#include <map>

namespace p2pnet {
namespace overlay {

class OverlayNodeDB {
	friend class OverlayNode;	// As OverlayNode will call moveNode

	std::map<crypto::Hash, OverlayNode*> nodes;

	void moveNode(const TH& from, const TH& to);
public:
	OverlayNodeDB();
	virtual ~OverlayNodeDB();

	std::set<OverlayNode*> getAllNodes();
	OverlayNode* getNode(const overlay::TH& th);

	void notifyKeysUpdated(std::pair<crypto::PrivateKeyDSA, TH> previous_keys, std::pair<crypto::PrivateKeyDSA, TH> new_keys);
};

} /* namespace overlay */
} /* namespace p2pnet */

#endif /* OVERLAYNODEDB_H_ */
