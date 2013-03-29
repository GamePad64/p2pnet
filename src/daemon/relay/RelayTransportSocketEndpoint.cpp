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
#include <sstream>

namespace p2pnet {
namespace relay {

RelayTransportSocketEndpoint::RelayTransportSocketEndpoint() {}
RelayTransportSocketEndpoint::~RelayTransportSocketEndpoint() {}

const crypto::hash_t& RelayTransportSocketEndpoint::getRelayHash() const {
	return relay_hash;
}

void RelayTransportSocketEndpoint::setRelayHash(
		const crypto::hash_t& relayHash) {
	relay_hash = relayHash;
}

RelayTransportSocketEndpoint::RelayTransportSocketEndpoint(
		crypto::hash_t relay_hash) {
	this->relay_hash = relay_hash;
}

std::string RelayTransportSocketEndpoint::toString() {
	return std::string(relay_hash.begin(), relay_hash.end());
}

void RelayTransportSocketEndpoint::fromString(std::string endpoint_s) {
	relay_hash = crypto::hash_t(endpoint_s.begin(), endpoint_s.end());
}

RelayTransportSocketEndpoint::RelayTransportSocketEndpoint(
		std::string endpoint_s) {
	this->fromString(endpoint_s);
}

std::string RelayTransportSocketEndpoint::toHRString(){
	std::ostringstream os;
	os << "TH:" << crypto::hashToHex(relay_hash);
	return os.str();
}

} /* namespace relay */
} /* namespace p2pnet */
