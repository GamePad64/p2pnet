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
#ifndef cb2d1228_b0d8_451a_962c_62d4e073ad81
#define cb2d1228_b0d8_451a_962c_62d4e073ad81
#pragma once

#include "SocketEndpoint.h"
#include "Socket.h"
#include "../../common/Config.h"
#include <memory>

namespace p2pnet {
namespace transport {

class InterfaceEndpoint;
class SocketEndpoint;

class Interface : protected ConfigClient, public std::enable_shared_from_this<Interface> {
	friend class Socket;
protected:
	std::weak_ptr<Socket> parent_socket;
public:
	Interface() {};
	virtual ~Interface(){};

	// Interface properties
	virtual std::string getPrefix() const = 0;
	virtual bool isLocalOnly() const = 0;

	virtual std::shared_ptr<InterfaceEndpoint> createEndpoint() = 0;

	// Interface asynchronous I/O functions.
	virtual void setReceiveCallback(Socket::ReceiveCallback callback) = 0;
	virtual void setDisconnectCallback(Socket::DisconnectCallback callback) = 0;

	std::shared_ptr<Socket> getParent(){return parent_socket.lock();}

	virtual void connect(SocketEndpoint endpoint, Socket::ConnectCallback callback) = 0;
};

} /* namespace net */
} /* namespace p2pnet */
#endif
