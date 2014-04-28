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
#include "NodeManager.h"

namespace p2pnet {
namespace p2p {

NodeManager::NodeManager() {}
NodeManager::~NodeManager() {
	for(auto bound_it : nodes_bound){
		delete bound_it.second;
	}
	for(auto unbound_it : nodes_unbound){
		delete unbound_it;
	}
}

void NodeManager::bindNode(Node* node, SH sh) {
	auto unbound_it = nodes_unbound.find(node);
	if(unbound_it != nodes_unbound.end()){
		nodes_bound.insert(std::make_pair(sh, node));
		nodes_unbound.erase(node);
	}else{
		auto bound_it = nodes_bound.find(node->getLocalSH());
		if(bound_it != nodes_bound.end()){
			nodes_bound.erase(bound_it);
			nodes_bound.insert(std::make_pair(sh, node));
		}else{
			// TODO: Exception model
			throw std::runtime_error("NodeManager's DB is inconsistent");
		}
	}
}

void NodeManager::unbindNode(Node* node) {
	auto sh = node->getLocalSH();
	auto bound_it = nodes_bound.find(node->getLocalSH());
	if(bound_it != nodes_bound.end()){
		nodes_bound.erase(bound_it);
		nodes_bound.insert(std::make_pair(sh, node));
	}
}

Node* NodeManager::createNode() {
}

Node* NodeManager::createNode(api::APISession* api_session) {
}

void NodeManager::createNode(Node* node) {
}

void NodeManager::destroyNode(Node* node) {
}

} /* namespace p2p */
} /* namespace p2pnet */
