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

TransportInterface* TransportSocket::getInterfaceByID(uint32_t id){
	return interfaces[id];
}
void TransportSocket::registerInterface(TransportInterface* interface){
	interfaces[interface->getInterfaceID()] = interface;
	readable_strings_prefixes[interface->getInterfacePrefix()] = interface;
}

void TransportSocket::updateOnReceive(MessageBundle bundle) {
	for(auto &transportsocketlistener : m_listenerlist){
		transportsocketlistener->receivedMessage(bundle);
	}
}

void TransportSocket::updateOnSend(MessageBundle bundle) {
	for(auto &transportsocketlistener : m_listenerlist){
		transportsocketlistener->sentMessage(bundle);
	}
}

}
}
