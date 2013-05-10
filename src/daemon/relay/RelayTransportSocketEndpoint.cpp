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

#include "RelayTransportSocketEndpoint.h"
#include "../../common/crypto/Hash.h"
#include <sstream>

namespace p2pnet {
namespace relay {

RelayTransportSocketEndpoint::~RelayTransportSocketEndpoint() {
}

const crypto::Hash& RelayTransportSocketEndpoint::getRelayTH() const {
	return m_relay_th;
}

void RelayTransportSocketEndpoint::setRelayTH(const crypto::Hash& relayHash) {
	m_relay_th = relayHash;
}

RelayTransportSocketEndpoint::RelayTransportSocketEndpoint(crypto::Hash relay_th) :
		m_relay_th(relay_th) {
}

databases::TransportSocketEndpoint_s RelayTransportSocketEndpoint::toProtobuf() const {
	databases::TransportSocketEndpoint_s tse_s;
	tse_s.set_type(databases::TransportSocketEndpoint_s::RELAY);

	tse_s.set_th(m_relay_th.toBinaryString());
	return tse_s;
}

RelayTransportSocketEndpoint::RelayTransportSocketEndpoint(databases::TransportSocketEndpoint_s tse_s) :
		m_relay_th(crypto::Hash::fromBinaryString(tse_s.th())) {
}

std::string RelayTransportSocketEndpoint::toHRString() {
	std::ostringstream os;
	os << "TH:" << m_relay_th.toBase58();
	return os.str();
}

} /* namespace relay */
} /* namespace p2pnet */
