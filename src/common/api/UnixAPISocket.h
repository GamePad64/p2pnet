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
#ifndef UNIXAPISOCKET_H_
#define UNIXAPISOCKET_H_

#include "APIMessage.pb.h"
#include "../Loggable.h"
#include <boost/asio.hpp>
#include <list>

#define MESSAGE_SIZE_TYPE uint32_t
#define MESSAGE_SIZE_LENGTH sizeof(MESSAGE_SIZE_TYPE)

using boost::asio::local::stream_protocol;

namespace p2pnet {
namespace api {
namespace unix {

std::list<std::string> getSocketPathList();

class UnixAPISocket : protected Loggable {
	stream_protocol::socket session_socket;
	std::function<void()> m_shutdown_func;
	std::function<void(APIMessage)> m_process_func;

	bool shut;

public:
	UnixAPISocket(boost::asio::io_service& io_service);
	virtual ~UnixAPISocket();

	stream_protocol::socket& getSocket();

	void assignShutdownHandler(std::function<void()> shutdown_handler);
	void assignReceiveHandler(std::function<void(APIMessage)> receive_handler);

	void startReceive();
	void handleReceiveSize(const boost::system::error_code& error, char* char_message_size);
	void handleReceive(const boost::system::error_code& error, char* message, uint32_t size);

	void send(APIMessage message);
	void handleSend(const boost::system::error_code& error);

	void cleanup();
};

} /* namespace unix */
} /* namespace api */
} /* namespace p2pnet */

#endif /* UNIXAPISOCKET_H_ */
