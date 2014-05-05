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
#ifndef NODEMANAGER_H_
#define NODEMANAGER_H_

#include "SH.h"
#include "../../common/Singleton.h"
#include <map>
#include <set>

namespace p2pnet {
namespace api {class APISession;}
namespace p2p {

class Node;

class NodeManager : public Singleton<NodeManager> {
	friend class Node;
	std::map<SH, Node*> nodes_bound;
	std::set<Node*> nodes_unbound;

	/**
	 * This function only affects NodeManager's DB.
	 * Sets specified SH to node.
	 * @param node
	 * @param sh
	 */
	void bindNode(Node* node, SH sh);
	/**
	 * This function only affects NodeManager's DB.
	 * Resets bound node's SH.
	 * @param node
	 * @param sh
	 */
	void unbindNode(Node* node);
public:
	NodeManager();
	virtual ~NodeManager();

	Node* createNode();
	Node* createNode(api::APISession* api_session);

	void destroyNode(Node* node);
};

} /* namespace p2p */
} /* namespace p2pnet */

#endif /* NODEMANAGER_H_ */
