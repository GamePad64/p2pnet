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

#include <boost/asio.hpp>
#include "LowLevelAPISession.h"

#define MESSAGE_SIZE_TYPE uint32_t
#define MESSAGE_SIZE_LENGTH sizeof(MESSAGE_SIZE_TYPE)

using boost::asio::local::stream_protocol;

namespace p2pnet {
namespace api {
namespace unix {

std::string getSocketPath();
std::string getFallbackSocketPath();

class UnixAPISocket {
	stream_protocol::socket session_socket;
	LowLevelAPISession* session_ptr;

public:
	UnixAPISocket(boost::asio::io_service& io_service, LowLevelAPISession* session);
	virtual ~UnixAPISocket();

	stream_protocol::socket& getSocket();

	void startReceive();
	void handleReceiveSize(const boost::system::error_code& error, char* char_message_size);
	void handleReceive(const boost::system::error_code& error, char* message, uint32_t size);

	void handleSend(const boost::system::error_code& error);

	void send(APIMessage message);
};

} /* namespace unix */
} /* namespace api */
} /* namespace p2pnet */

#endif /* UNIXAPISOCKET_H_ */
