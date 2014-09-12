/* * You may redistribute this program and/or modify it under the terms of * the GNU General Public License as published by the Free Software Foundation, * either version 3 of the License, or (at your option) any later version. * * This program is distributed in the hope that it will be useful, * but WITHOUT ANY WARRANTY; without even the implied warranty of * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the * GNU General Public License for more details. * * You should have received a copy of the GNU General Public License * along with this program.  If not, see <http://www.gnu.org/licenses/>. */#pragma once#include <string>#include <memory>#include <map>#include "../../common/Loggable.h"namespace p2pnet {namespace transport {class SocketEndpoint;class Connection;class Interface;class Socket : public std::enable_shared_from_this<Socket>, Loggable {protected:	std::map<std::string, std::shared_ptr<Interface>> interfaces;	std::map<SocketEndpoint, std::shared_ptr<Connection>> connections;	std::set<SocketEndpoint> banned_peers;public:	using SendCallback = std::function<void(int, std::shared_ptr<Connection>, std::string, int)>;	// void(int error_code, SocketEndpoint endpoint, std::string message, int bytes_transferred)	using ReceiveCallback = std::function<void(int, std::shared_ptr<Connection>, std::string)>;	using ConnectCallback = std::function<void(int, std::shared_ptr<Connection>)>;	using DisconnectCallback = std::function<void(int, std::shared_ptr<Connection>)>;	Socket();	virtual ~Socket();	std::shared_ptr<Interface> getInterface(std::string prefix);	void registerInterface(std::shared_ptr<Interface> interface);	// Basically, asynchronous I/O	void setReceiveCallback(ReceiveCallback callback);	void setDisconnectCallback(DisconnectCallback callback);	// Something for interfaces	std::shared_ptr<Connection> getConnection(SocketEndpoint endpoint);	// Nullable. If nullptr, then use connect();	void registerConnection(std::shared_ptr<Connection> connection);	void unregisterConnection(std::shared_ptr<Connection> connection);	// For everybody	void connect(SocketEndpoint endpoint, ConnectCallback callback);	// TODO: Exceptions or errno	void ban(SocketEndpoint endpoint);	// TODO	void unban(SocketEndpoint endpoint);	// TODO};} /* namespace net */} /* namespace p2pnet */