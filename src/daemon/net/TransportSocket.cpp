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

#include "TransportSocket.h"
#include "TransportSocketListener.h"
#include "TransportSocketEndpoint.h"

namespace p2pnet {
namespace net {

TransportSocket::~TransportSocket(){};

MessageBundle TransportSocket::createMessageBundle(std::string message,
		TransportSocketEndpoint &endpoint,
		TransportSocketConnection connection) {
	MessageBundle bundle;
	if (!connection.isNull()) {
		bundle.socket_connection = connection;
	} else {
		TransportSocketConnection socket_connection(this, &endpoint);
		bundle.socket_connection = socket_connection;
	}
	bundle.message = message;
	return bundle;
}

}
}
