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

namespace p2pnet {
namespace net {

class TransportSocketEndpoint {
	TransportInterfaceEndpoint::pointer interface_endpoint;
public:
	TransportSocketEndpoint(){};
	~TransportSocketEndpoint(){};

	uint32_t getInterfaceID() const;
	TransportInterface* getInterfaceByID(uint32_t id){
		return TransportSocket::getInstance()->getInterfaceByID(id);
	}

	/*
	 * Serialization
	 */
	void fromProtobuf(databases::TransportSocketEndpoint_s tse_s) = 0;
	databases::TransportSocketEndpoint_s toProtobuf() const = 0;
	TransportSocketEndpoint(databases::TransportSocketEndpoint_s tse_s);

	void fromBinaryString(std::string binary_string);
	inline std::string toBinaryString() const {
		return toProtobuf().SerializeAsString();
	}

	/*
	 * Readable strings.
	 */
	std::string toReadableString() const = 0;
	void fromReadableString(std::string readable_string) const = 0;
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* TRANSPORTSOCKETENDPOINT_H_ */
