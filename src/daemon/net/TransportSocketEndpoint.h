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

#ifndef TRANSPORTSOCKETENDPOINT_H_
#define TRANSPORTSOCKETENDPOINT_H_

#include <string>
#include <memory>
#include "../protobuf/NetDBEntry.pb.h"
#include "TransportInterface.h"

namespace p2pnet {
namespace net {

class TransportSocketEndpoint {
	TransportInterfaceEndpoint::pointer interface_endpoint;
	/**
	 * Creates new endpoint instance using specified interface ID.
	 * @param id
	 */
	void resetEndpointByID(uint32_t id);
public:
	TransportSocketEndpoint(){};
	TransportSocketEndpoint(const TransportSocketEndpoint& tse);
	~TransportSocketEndpoint(){};

	// Operators
	void operator =(const TransportSocketEndpoint& tse);

	explicit operator bool(){
		return bool(interface_endpoint);
	}

	uint32_t getInterfaceID() const {
		return bool(interface_endpoint) ? interface_endpoint->getInterfaceID() : 0;
	}
	TransportInterface* getInterfaceByID(uint32_t id);

	/*
	 * Serialization
	 */
	void fromProtobuf(databases::TransportSocketEndpoint_s tse_s);
	databases::TransportSocketEndpoint_s toProtobuf() const;
	TransportSocketEndpoint(databases::TransportSocketEndpoint_s tse_s);

	/*
	 * Binary strings serialization. Uses protobuf (look up! ^_^)
	 */
	void fromBinaryString(std::string binary_string);
	inline std::string toBinaryString() const {
		return toProtobuf().SerializeAsString();
	}

	/*
	 * Human readable strings operation.
	 */
	void fromReadableString(std::string readable_string);
	std::string toReadableString() const;
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* TRANSPORTSOCKETENDPOINT_H_ */
