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
#ifndef P2PDAEMON_H_
#define P2PDAEMON_H_

#include "../common/api/APIMessage.pb.h"
#include "../p2pnet.h"

#include <future>

namespace p2pnet {

class P2PDaemon {
protected:
	typedef std::function<void(std::error_condition&)> SendHandler;
	typedef std::function<void(api::APIMessage, std::error_condition&)> ReceiveHandler;

	std::thread* socket_thread;
	boost::asio::io_service m_io_service;

	std::map<uint32_t, std::pair<std::mutex*, api::APIMessage*>> awaiting_receive;
	uint32_t next_msg_token = 0;

	void handleSend(uint32_t message_token, std::error_condition& error);
	void handleReceive(api::APIMessage message, std::error_condition& error);

	void startReceiveLoop();

	/**
	 * Adds token number to message
	 * @param message
	 * @return token
	 */
	uint32_t markMessage(api::APIMessage& message);
public:
	P2PDaemon();
	virtual ~P2PDaemon(){};

	// We use asynchronous I/O.
	virtual void asyncSend(api::APIMessage data, SendHandler handler) = 0;
	virtual void asyncReceive(ReceiveHandler handler) = 0;

	api::APIMessage clientExchange(api::APIMessage request_message);

	virtual void connect() = 0;
	virtual bool is_connected() = 0;
	virtual int disconnect() = 0;
};

} /* namespace p2pnet */

#endif /* P2PDAEMON_H_ */
