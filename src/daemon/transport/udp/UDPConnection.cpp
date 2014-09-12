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
#include "UDPConnection.h"
#include "UDPInterface.h"
#include "../Errors.h"

#include "../../AsioIOService.h"

namespace p2pnet {
namespace transport {

UDPConnection::UDPConnection(SocketEndpoint endpoint, std::shared_ptr<Interface> parent_interface) : Connection(parent_interface),
		endpoint(endpoint),
		conn_send_timer(AsioIOService::getIOService()),
		conn_receive_timeout(AsioIOService::getIOService()) {
}

UDPConnection::~UDPConnection() {}

void UDPConnection::sendKeepAlive(const boost::system::error_code& error) {
	if(error != boost::asio::error::operation_aborted){
		auto udp_interface = std::static_pointer_cast<UDPInterface>(getParent());
		send(UDP_KEEPALIVE_MSG, [](int, std::shared_ptr<Connection>, std::string, int){});

		conn_send_timer.expires_from_now(std::chrono::seconds(udp_interface->getKeepaliveTimer()));
		conn_send_timer.async_wait(boost::bind(&UDPConnection::sendKeepAlive, this, boost::asio::placeholders::error));
	}
}

void UDPConnection::receivedConnect() {
	m_connected = true;
	onConnect();
	sendKeepAlive(boost::system::error_code());
}

void UDPConnection::receivedKeepAlive() {
	auto udp_interface = std::static_pointer_cast<UDPInterface>(getParent());

	conn_receive_timeout.expires_from_now(std::chrono::seconds(udp_interface->getInactivityTimeout()));
	conn_receive_timeout.async_wait([&, this](const boost::system::error_code& error){
		if(error != boost::asio::error::operation_aborted){
			udp_interface->getDisconnectCallback()(timed_out, shared_from_this());
			m_connected = false;
			onDisconnect();
		}
	});
	m_connected = true;
}

void UDPConnection::receivedDisconnect(){
	auto udp_interface = std::static_pointer_cast<UDPInterface>(getParent());

	conn_send_timer.cancel();
	conn_receive_timeout.cancel();

	udp_interface->getDisconnectCallback()(connection_reset, shared_from_this());
	m_connected = false;
	onDisconnect();
}

void UDPConnection::connect(Socket::ConnectCallback callback){
	auto udp_interface = std::static_pointer_cast<UDPInterface>(getParent());

	conn_receive_timeout.expires_from_now(std::chrono::seconds(udp_interface->getInactivityTimeout()));
	conn_receive_timeout.async_wait([&](const boost::system::error_code& error){
		if(error != boost::asio::error::operation_aborted){
			connecting_callback(timed_out, shared_from_this());
			m_connected = false;
		}else{
			connecting_callback(0, shared_from_this());
			m_connected = true;
			onConnect();
			sendKeepAlive(boost::system::error_code());
		}
	});

	send(UDP_CONNECT_MSG, [](int, std::shared_ptr<Connection>, std::string, int){});
}

void UDPConnection::send(const std::string& data, Socket::SendCallback callback) {
	auto udp_interface = std::static_pointer_cast<UDPInterface>(getParent());
	udp_interface->send(shared_from_this(), data, callback);
}

bool UDPConnection::connected() const {
	return m_connected;
}

} /* namespace transport */
} /* namespace p2pnet */
