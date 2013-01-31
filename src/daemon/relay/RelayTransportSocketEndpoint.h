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

#ifndef RELAYTRANSPORTSOCKETENDPOINT_H_
#define RELAYTRANSPORTSOCKETENDPOINT_H_

#include "../../common/crypto/CryptoTypes.h"
#include "../net/TransportSocketEndpoint.h"

namespace p2pnet {
namespace relay {

class RelayTransportSocketEndpoint: public p2pnet::net::TransportSocketEndpoint {
	crypto::hash_t relay_hash;
public:
	RelayTransportSocketEndpoint();
	virtual ~RelayTransportSocketEndpoint();

	const crypto::hash_t& getRelayHash() const;
	void setRelayHash(const crypto::hash_t& relayHash);
	RelayTransportSocketEndpoint(crypto::hash_t relay_hash);

	virtual std::string toString();
	virtual void fromString(std::string endpoint_s);
	RelayTransportSocketEndpoint(std::string endpoint_s);
};

} /* namespace relay */
} /* namespace p2pnet */
#endif /* RELAYTRANSPORTSOCKETENDPOINT_H_ */
