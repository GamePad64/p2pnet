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

#ifndef TRANSPORTSOCKETMESSAGEDISPATCHER_H_
#define TRANSPORTSOCKETMESSAGEDISPATCHER_H_

#include "../net/TransportSocketListener.h"

namespace p2pnet {
namespace messaging {

class TransportSocketMessageDispatcher: public p2pnet::net::TransportSocketListener {
public:
	TransportSocketMessageDispatcher();
	virtual ~TransportSocketMessageDispatcher();

	void receivedMessage(net::MessageBundle message_bundle);
	void sentMessage(net::MessageBundle message_bundle);
};

} /* namespace messaging */
} /* namespace p2pnet */
#endif /* TRANSPORTSOCKETMESSAGEDISPATCHER_H_ */
