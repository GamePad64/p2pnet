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

#ifndef TRANSPORTINTERFACEENDPOINT_H_
#define TRANSPORTINTERFACEENDPOINT_H_

#include "../protobuf/NetDBEntry.pb.h"

namespace p2pnet {
namespace net {

class TransportInterfaceEndpoint {
public:
	TransportInterfaceEndpoint(){};
	virtual ~TransportInterfaceEndpoint(){};

	typedef std::shared_ptr<TransportInterfaceEndpoint> pointer;
	typedef std::shared_ptr<const TransportInterfaceEndpoint> const_pointer;

	virtual uint32_t getInterfaceID() const {return 0;};

	/*
	 * Serialization
	 */
	virtual void fromProtobuf(databases::TransportSocketEndpoint_s tse_s) = 0;
	virtual databases::TransportSocketEndpoint_s toProtobuf() const = 0;
	TransportInterfaceEndpoint(databases::TransportSocketEndpoint_s tse_s);

	void fromBinaryString(std::string binary_string);
	std::string toBinaryString() const;

	/*
	 * Readable strings.
	 */
	virtual std::string toReadableString() const = 0;
	virtual void fromReadableString(std::string readable_string) = 0;
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* TRANSPORTINTERFACEENDPOINT_H_ */
