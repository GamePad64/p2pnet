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
#pragma once

#include "SocketEndpoint.h"
#include "Socket.h"
#include "../../common/Loggable.h"

namespace p2pnet {
namespace transport {

class Connection : public std::enable_shared_from_this<Connection>, protected Loggable {
	std::weak_ptr<Interface> parent_interface;
protected:
	Connection() = delete;
	Connection(std::shared_ptr<Interface> parent_interface);
public:
	virtual ~Connection(){};

	virtual SocketEndpoint getEndpoint() = 0;
	std::shared_ptr<Interface> getParent(){return parent_interface.lock();}

	virtual void connect(Socket::ConnectCallback callback) = 0;
	virtual void send(const std::string& data, Socket::SendCallback callback) = 0;
	virtual bool connected() const = 0;

	virtual void onConnect();
	virtual void onDisconnect();
};

} /* namespace net */
} /* namespace p2pnet */
