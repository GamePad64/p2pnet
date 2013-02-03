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

#ifndef TRANSPORTSOCKETCONNECTION_H_
#define TRANSPORTSOCKETCONNECTION_H_

#include "TransportSocketEndpoint.h"
#include <memory>

namespace p2pnet {
namespace net {

class TransportSocket;
class TransportSocketListener;
class MessageBundle;

/*!
 * A class that wraps TransportSocket with a defined endpoint. It is a class that makes sending packets over different sockets more generic.
 */
class TransportSocketConnection {
	TransportSocket* m_socket;
	TransportSocketEndpoint::pointer m_endpoint_p;
public:
	TransportSocketConnection(TransportSocket* socket, TransportSocketEndpoint::pointer endpoint);
	virtual ~TransportSocketConnection();

	TransportSocketEndpoint::pointer getEndpointPtr(){return m_endpoint_p;};

	inline void asyncReceiveFrom();
	inline void asyncSendTo(const std::string& data);

	inline void waitReceiveFrom();
	inline void waitSendTo(const std::string& data);

	inline MessageBundle hereReceiveFrom();
	inline void hereSendTo(const std::string& data);
};

} /* namespace net */
} /* namespace p2pnet */

#endif /* TRANSPORTSOCKETCONNECTION_H_ */
