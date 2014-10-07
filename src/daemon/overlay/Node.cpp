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
#include "Node.h"
#include "OverlayNode.pb.h"

#include "../AsioIOService.h"
#include "OverlaySocket.h"
#include "OverlayConnection.h"

#include "OverlayNode.pb.h"

#include <boost/date_time.hpp>

namespace p2pnet {
namespace overlay {

Node::Node(const crypto::Hash& node_th) {
	node_keyinfo.th = node_th;
}

Node::Node(const std::string& serialized_contact) {
	putSerializedContact(serialized_contact);
}

Node::~Node() {
}

crypto::Hash Node::getHash() const {
	return node_keyinfo.th;
}

KeyInfo Node::getKeyInfo() const {
	return node_keyinfo;
}

void Node::setPublicKey(const crypto::PublicKeyDSA& public_key) {
	node_keyinfo.public_key = public_key;
	auto new_th = crypto::Hash(public_key);

	Socket::getInstance()->getNodeDB()->moveNode(node_keyinfo.th, new_th);

	node_keyinfo.th = new_th;
}

std::string Node::getSerializedContact() const {
	OverlayNode_s node_s;
	node_s.set_node_th(node_keyinfo.th.toBinaryString());
	if(node_keyinfo.public_key.isPresent()){
		node_s.set_public_key(node_keyinfo.public_key.toBinaryString());
	}

	node_s.set_expires(std::chrono::system_clock::to_time_t(node_keyinfo.expiration_time));
	node_s.set_lost(std::chrono::system_clock::to_time_t(node_keyinfo.lose_time));

	auto tse_list = getTransportEndpoints();	// TODO: Send about expired.
	for(auto tse_it : tse_list){
		node_s.mutable_endpoints()->add_tse_s()->CopyFrom(tse_it.toProtobuf());
	}
	return std::string(node_s.SerializeAsString());
}

void Node::putSerializedContact(const std::string& serialized_contact) {
	//TODO
}

std::chrono::seconds Node::getLastActivityAge() const {
	return std::chrono::duration_cast<std::chrono::seconds>(system_clock::now()-last_activity);
}

void Node::bumpLastActivity(){
	last_activity = system_clock::now();
}

Node::Reliability Node::getReliability() const {
	if(getLastActivityAge() < std::chrono::minutes(15)){
		return Reliability::GOOD;
	}else{
		return Reliability::QUESTIONABLE;
	}
	//TODO: BAD nodes
}

void Node::updateEndpoint(const transport::SocketEndpoint& endpoint, bool verified){
	auto it = std::find(transport_endpoints.begin(), transport_endpoints.end(), endpoint);

	if(it != transport_endpoints.end())
		transport_endpoints.erase(it);

	if(verified)
		transport_endpoints.push_front(endpoint);
	else
		transport_endpoints.push_back(endpoint);
}

void Node::updateExpirationTime(std::chrono::system_clock::time_point expiry_time) {
	if(expiry_time > node_keyinfo.expiration_time)	// This is to prevent spoofing.
		node_keyinfo.expiration_time = expiry_time;
}

void Node::updateLoseTime(std::chrono::system_clock::time_point lost_time) {
	node_keyinfo.lose_time = lost_time;
}

} /* namespace overlay */
} /* namespace p2pnet */
