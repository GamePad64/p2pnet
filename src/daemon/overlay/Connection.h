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
#ifndef OVERLAYCONNECTION_H_
#define OVERLAYCONNECTION_H_

using std::chrono::system_clock;

namespace p2pnet {
using namespace protocol;
namespace overlay {

class Connection : public std::enable_shared_from_this<Connection>, protected Loggable {
	std::weak_ptr<Socket> parent_socket;
protected:
	Connection() = delete;
	Connection(std::shared_ptr<Interface> parent_interface);
public:
	virtual ~Connection(){};

	virtual SocketEndpoint getEndpoint() = 0;
	std::shared_ptr<Socket> getParent(){return parent_socket.lock();}

	void connect(Socket::ConnectCallback callback) = 0;
	void send(const std::string& data, Socket::SendCallback callback) = 0;
	bool connected();

	virtual void onConnect();
	virtual void onDisconnect();
};

} /* namespace overlay */
} /* namespace p2pnet */

#endif /* OVERLAYCONNECTION_H_ */
