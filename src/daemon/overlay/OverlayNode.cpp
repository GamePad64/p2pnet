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
#include "OverlayNode.h"
#include "../protobuf/OverlayNode.pb.h"

#include "../AsioIOService.h"
#include "OverlaySocket.h"
#include "OverlayConnection.h"

#include <boost/date_time.hpp>

namespace p2pnet {
namespace overlay {

OverlayNode::OverlayNode(const crypto::Hash& node_th) : key_lose_timer(AsioIOService::getIOService()) {
	th = node_th;
	connection = nullptr;
}

OverlayNode::OverlayNode(const std::string& serialized_contact) : key_lose_timer(AsioIOService::getIOService()) {
	putSerializedContact(serialized_contact);
	connection = nullptr;
}

OverlayNode::~OverlayNode() {
	delete connection;
}

crypto::Hash OverlayNode::getHash() const {
	return th;
}

crypto::PublicKeyDSA OverlayNode::getPublicKey() const {
	return public_key;
}

void OverlayNode::setPublicKey(const crypto::PublicKeyDSA& public_key) {
	this->public_key = public_key;
	auto new_th = crypto::Hash(public_key);

	OverlaySocket::getInstance()->getNodeDB()->moveNode(th, new_th);

	th = new_th;
}

OverlayConnection* OverlayNode::getConnection() {
	if(!connection)
		connection = new OverlayConnection(this);
	return connection;
}

bool OverlayNode::hasConnection() const {
	return connection != nullptr;
}

std::string OverlayNode::getSerializedContact() const {
	OverlayNode_s node_s;
	node_s.set_node_th(th.toBinaryString());
	if(public_key.isPresent()){
		node_s.set_public_key(public_key.toBinaryString());
	}

	node_s.set_expires(getISOExpirationTime());
	const auto& tse_list = it->second->getTransportEndpoints();	// TODO: Send about expired.
	for(auto tse_it : tse_list){
		tse_s.add_tse_s()->CopyFrom(tse_it.toProtobuf());
	}
	return std::string(tse_s.SerializeAsString());
}

void OverlayNode::putSerializedContact(const std::string& serialized_contact) {
	//TODO
}

std::chrono::seconds OverlayNode::getLastActivityAge() const {
	return system_clock::now()-last_activity;
}

void OverlayNode::bumpLastActivity(){
	last_activity = system_clock::now();
}

OverlayNode::Reliability OverlayNode::getReliability() const {
	if(getLastActivityAge() < std::chrono::minutes(15)){
		return Reliability::GOOD;
	}else{
		return Reliability::QUESTIONABLE;
	}
	//TODO: BAD nodes
}

bool OverlayNode::isExchangeable() const {
	//TODO
	return true;
}

void OverlayNode::updateEndpoint(const transport::TransportSocketEndpoint& endpoint, bool verified){
	auto it = std::find(transport_endpoints.begin(), transport_endpoints.end(), endpoint);

	if(it != transport_endpoints.end())
		transport_endpoints.erase(it);

	if(verified)
		transport_endpoints.push_front(endpoint);
	else
		transport_endpoints.push_back(endpoint);
}

void OverlayNode::updateExpirationTime(std::chrono::system_clock::time_point expiry_time) {
	if(expiry_time > key_expiration_time)	// This is to prevent spoofing.
		key_expiration_time = expiry_time;
}

void OverlayNode::updateLoseTime(std::chrono::system_clock::time_point lost_time) {
	if(lost_time > key_lose_time){
		key_lose_time = lost_time;
		key_lose_timer.expires_at(key_lose_time);
		key_lose_timer.async_wait([&](boost::system::error_code ec){if(!ec && connection){connection->disconnect();}});
	}
}

void OverlayNode::registerDHT() {
}

void OverlayNode::unregisterDHT() {
}

OverlayNode& OverlayNode::operator =(const OverlayNode& other) {
}

} /* namespace overlay */
} /* namespace p2pnet */
