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
#ifndef UNIXCLIENTDATASOCKET_H_
#define UNIXCLIENTDATASOCKET_H_

namespace p2pnet {
namespace impl {

class UnixClientDataSocket : public ClientDataSocket {
	boost::asio::io_service& m_io_service;
	api::unix::UnixAPISocket socket;
public:
	UnixClientDataSocket(std::string socket_addr, boost::asio::io_service& io_service);
	virtual ~UnixClientDataSocket();

	void send(api::APIMessage data, int& error_code);
	api::APIMessage receive(int& error_code);

	void asyncSend(api::APIMessage data, SendHandler send_handler);
	void asyncReceive(ReceiveHandler receive_handler);
};

} /* namespace impl */
} /* namespace p2pnet */

#endif /* UNIXCLIENTDATASOCKET_H_ */
