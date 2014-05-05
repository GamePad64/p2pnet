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

#include "api_handlers.h"
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

	std::error_condition stdErrorFromBoostError(boost::system::error_code boost_error);
	std::string generateSendString(std::string from);

	void handleSend(const boost::system::error_code& boost_error,
			std::size_t bytes_transferred,
			SendHandler send_handler);

	void handleReceiveSize(const boost::system::error_code& boost_error,
			std::size_t bytes_transferred,
			char* char_message_size,
			ReceiveHandler receive_handler);
	void handleReceive(const boost::system::error_code& boost_error,
			char* message, uint32_t size,
			ReceiveHandler receive_handler);
public:
	UnixAPISocket(boost::asio::io_service& io_service);
	virtual ~UnixAPISocket();

	stream_protocol::socket& getSocket();

	void send(api::APIMessage data, std::error_condition& error);
	api::APIMessage receive(std::error_condition& error);

	void asyncSend(api::APIMessage data, SendHandler send_handler);
	void asyncReceive(ReceiveHandler receive_handler);
};

} /* namespace unix */
} /* namespace api */
} /* namespace p2pnet */

#endif /* UNIXAPISOCKET_H_ */
