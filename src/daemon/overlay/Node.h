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
#ifndef OVERLAYNODE_H_
#define OVERLAYNODE_H_

#include "../dht/DHTNode.h"
#include "../transport/SocketEndpoint.h"

#include "KeyProvider.h"
#include "TH.h"

#include <boost/asio.hpp>
#include <deque>

namespace p2pnet {
namespace overlay {

using std::chrono::system_clock;

class Node : public dht::DHTNode, public std::enable_shared_from_this<Node> {
	std::chrono::system_clock::time_point last_activity;

	KeyInfo node_keyinfo;

	std::deque<transport::SocketEndpoint> transport_endpoints;
public:
	Node(const crypto::Hash& node_th);
	Node(const std::string& serialized_contact);
	virtual ~Node();

	// Cryptographic/Identity functions
	virtual crypto::Hash getHash() const;

	// KeyInfo manipulation
	KeyInfo getKeyInfo() const;
	void setPublicKey(const crypto::PublicKeyDSA& public_key);

	void updateExpirationTime(system_clock::time_point expiry_time);
	void updateLoseTime(system_clock::time_point lost_time);

	// Serialization functions
	virtual std::string getSerializedContact() const;
	virtual void putSerializedContact(const std::string& serialized_contact);

	// Last activity functions
	virtual std::chrono::seconds getLastActivityAge() const;
	void bumpLastActivity();

	virtual Reliability getReliability() const;

	// TransportEndpoint functions
	decltype(transport_endpoints) getTransportEndpoints() const {return transport_endpoints;}
	void updateEndpoint(const transport::SocketEndpoint& endpoint, bool verified = false);
};

} /* namespace overlay */
} /* namespace p2pnet */

#endif /* OVERLAYNODE_H_ */
