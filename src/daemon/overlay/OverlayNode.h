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
#include "../transport/TransportSocketEndpoint.h"

#include "../../common/crypto/PublicKeyDSA.h"
#include "TH.h"

#include <boost/asio.hpp>
#include <deque>

namespace p2pnet {
namespace overlay {

using std::chrono::system_clock;

class OverlayConnection;
class OverlayNode : public dht::DHTNode {
	OverlayConnection* connection;

	std::chrono::system_clock::time_point last_activity;
	std::chrono::system_clock::time_point key_expiration_time;
	std::chrono::system_clock::time_point key_lose_time;
	boost::asio::deadline_timer key_lose_timer;

	crypto::PublicKeyDSA public_key;
	TH th;

	std::deque<transport::TransportSocketEndpoint> transport_endpoints;
public:
	OverlayNode(const crypto::Hash& node_th);
	OverlayNode(const std::string& serialized_contact);
	virtual ~OverlayNode();

	// Cryptographic/Identity functions
	virtual crypto::Hash getHash() const;

	crypto::PublicKeyDSA getPublicKey() const;
	void setPublicKey(const crypto::PublicKeyDSA& public_key);

	// Connection functions
	OverlayConnection* getConnection();
	bool hasConnection() const;

	// Serialization functions
	virtual std::string getSerializedContact() const;
	virtual void putSerializedContact(const std::string& serialized_contact);

	// Last activity functions
	virtual std::chrono::seconds getLastActivityAge() const;
	void bumpLastActivity();

	virtual Reliability getReliability() const;
	virtual bool isExchangeable() const;

	// TransportEndpoint functions
	decltype(transport_endpoints) getTransportEndpoints(){return transport_endpoints;};
	void updateEndpoint(const transport::TransportSocketEndpoint& endpoint, bool verified = false);

	// Timing functions
	void updateExpirationTime(system_clock::time_point expiry_time);
	void updateLoseTime(system_clock::time_point lost_time);

	system_clock::time_point getExpirationTime();
	system_clock::time_point getLoseTime();

	// DHT functions
	void registerDHT();
	void unregisterDHT();

	OverlayNode& operator=(const OverlayNode& other);
};

} /* namespace overlay */
} /* namespace p2pnet */

#endif /* OVERLAYNODE_H_ */
