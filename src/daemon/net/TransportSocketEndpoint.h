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

namespace p2pnet {
namespace net {

class TransportSocketEndpoint {
public:
	TransportSocketEndpoint();
	virtual ~TransportSocketEndpoint();

	typedef std::shared_ptr<TransportSocketEndpoint> pointer;

	virtual TransportSocketEndpoint::pointer yieldCopyPtr() const = 0;

	/*!
	 * Returns Protocol Buffers representation of this TransportSocketEndpoint.
	 * @return binary std::string
	 */
	virtual std::string toString() = 0;
	/*!
	 * This function restores state of TransportSocketEndpoint from Protocol Buffers representation.
	 * @param endpoint_s std::string in binary form
	 */
	virtual void fromString(std::string endpoint_s) = 0;
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* TRANSPORTSOCKETENDPOINT_H_ */
