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
#include "OverlayNodeDB.h"
#include "OverlayConnection.h"
#include <boost/range/adaptor/map.hpp>

namespace p2pnet {
namespace overlay {

OverlayNodeDB::OverlayNodeDB() {}
OverlayNodeDB::~OverlayNodeDB() {
	for(auto node_ptr : boost::adaptors::values(nodes)){
		delete node_ptr;
	}
}

void OverlayNodeDB::moveNode(const overlay::TH& from, const overlay::TH& to) {
	nodes[to] = nodes[from];
}

std::set<OverlayNode*> OverlayNodeDB::getAllNodes() {
	std::set<OverlayNode*> node_set;

	for(auto node_ptr : boost::adaptors::values(nodes)){
		node_set.insert(node_ptr);
	}

	return node_set;
}

OverlayNode* OverlayNodeDB::getNode(const overlay::TH& th) {
	auto it_peer = nodes.find(th);

	if(it_peer != nodes.end()){
		return it_peer->second;
	}

	auto node_ptr = new OverlayNode(th);
	nodes.insert(std::make_pair(th, node_ptr));

	return node_ptr;
}

void OverlayNodeDB::notifyKeysUpdated(std::pair<crypto::PrivateKeyDSA, TH> previous_keys, std::pair<crypto::PrivateKeyDSA, TH> new_keys){
	for(auto node : boost::adaptors::values(nodes)){
		if(node->hasConnection()){
			node->getConnection()->performRemoteKeyRotation(previous_keys);
		}
	}
}

} /* namespace overlay */
} /* namespace p2pnet */
