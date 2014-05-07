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
#ifndef P2PSESSION_H_
#define P2PSESSION_H_

#include "../common/api/api_handlers.h"
#include "../common/api/APIMessage.pb.h"

#include <boost/asio.hpp>

#include <memory>
#include <map>
#include <thread>
#include <future>

namespace p2pnet {

class P2PDaemon;
class P2PSession {
	P2PDaemon* parent_daemon;

	std::thread* socket_thread;
	boost::asio::io_service m_io_service;
	std::unique_ptr<boost::asio::io_service::work> m_io_service_work;

	std::map<uint32_t, std::promise<api::APIMessage>> awaiting_receive;
	uint32_t next_msg_token = 0;

	void handleSend(std::error_condition& error);
	void handleReceive(api::APIMessage message, std::error_condition& error);

	/**
	 * Adds token number to message
	 * @param message
	 * @return token
	 */
	uint32_t markMessage(api::APIMessage& message);
public:
	P2PSession(P2PDaemon* parent_daemon);
	virtual ~P2PSession();

	api::SendHandler getSendHandler();
	api::ReceiveHandler getReceiveHandler();
	boost::asio::io_service& getIOService();

	void asyncSend(api::APIMessage data, api::SendHandler handler);
	void asyncReceive(api::ReceiveHandler handler);

	void receiveLoop();
	api::APIMessage clientExchange(api::APIMessage request_message);
};

} /* namespace p2pnet */

#endif /* P2PSESSION_H_ */
