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

#include "../../common/crypto/Hash.h"
#include "../net/TransportSocketEndpoint.h"

namespace p2pnet {
namespace relay {

class RelayTransportSocketEndpoint: public net::TransportSocketEndpoint {
	crypto::Hash m_relay_th;
public:
	virtual ~RelayTransportSocketEndpoint();

	const crypto::Hash& getRelayTH() const;
	void setRelayTH(const crypto::Hash& relay_th);
	RelayTransportSocketEndpoint(crypto::Hash relay_th);

	// Inherited from TransportSocketEndpoint
	virtual TransportSocketEndpoint::pointer yieldCopyPtr() const {
		TransportSocketEndpoint::pointer copy = std::make_shared<RelayTransportSocketEndpoint>(RelayTransportSocketEndpoint(*this));
		return copy;
	}
	virtual databases::TransportSocketEndpoint_s::Type getEndpointType() const {
		return databases::TransportSocketEndpoint_s::RELAY;
	}

	virtual databases::TransportSocketEndpoint_s toProtobuf() const;
	RelayTransportSocketEndpoint(databases::TransportSocketEndpoint_s tse_s);

	virtual std::string toHRString();
};

} /* namespace relay */
} /* namespace p2pnet */
#endif /* RELAYTRANSPORTSOCKETENDPOINT_H_ */
