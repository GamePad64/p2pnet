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
#include "UDPTransportConnection.h"
#include "UDPTransportInterface.h"

#include "../../AsioIOService.h"

namespace p2pnet {
namespace transport {

UDPTransportConnection::UDPTransportConnection(TransportSocketEndpoint endpoint, UDPTransportInterface* parent_interface) :
		TransportConnection(endpoint),
		conn_send_timer(AsioIOService::getIOService()),
		conn_receive_timeout(AsioIOService::getIOService()) {
	m_parent_interface = parent_interface;

	sendKeepAlive();
}

UDPTransportConnection::~UDPTransportConnection() {}

void UDPTransportConnection::sendKeepAlive() {
	send(KEEP_ALIVE_MSG);

	conn_send_timer.expires_from_now(boost::posix_time::seconds(SEND_TIMER));
	conn_send_timer.async_wait(std::bind(&UDPTransportConnection::sendKeepAlive, this));
}

void UDPTransportConnection::send(std::string data) {
	m_parent_interface->send(m_endpoint, data);
}

void UDPTransportConnection::process(std::string data) {
	conn_receive_timeout.expires_from_now(boost::posix_time::seconds(RECEIVE_TIMEOUT));
	conn_receive_timeout.async_wait([&](const boost::system::error_code& error){
		if(error != boost::asio::error::operation_aborted)
			m_connected = false;
	});
	m_connected = true;

	if(data != KEEP_ALIVE_MSG)
		this->TransportConnection::process(data);
}

bool UDPTransportConnection::connected() const {
	return m_connected;
}

} /* namespace transport */
} /* namespace p2pnet */
