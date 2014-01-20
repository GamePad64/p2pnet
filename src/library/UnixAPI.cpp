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
#include "UnixAPI.h"
#include "../common/api/APIMessage.pb.h"
#include "export.h"
#include <list>

namespace p2pnet {
namespace api {
namespace unix {

UnixAPI::UnixAPI(boost::asio::io_service& io_service) : socket(io_service) {
	socket.assignReceiveHandler(std::bind(&UnixAPI::process, this, std::placeholders::_1));
	socket.assignShutdownHandler([](){});

	connect();

	socket.startReceive();
}

UnixAPI::~UnixAPI() {}

void UnixAPI::process(APIMessage message) {
	log() << message.DebugString();
}
void UnixAPI::send(APIMessage message) {socket.send(message);}

void UnixAPI::connect() {
	bool connected = false;

	auto path_list = unix::getSocketPathList();
	auto it = path_list.begin();
	while((!connected) && it != path_list.end()){
		try {
			socket_path = *it;
			socket.getSocket().connect(stream_protocol::endpoint(socket_path));
			connected = true;
		} catch (boost::system::system_error& e) {
			if(++it == path_list.end())	// sic! increment is here.
				throw;	// We can do nothing. No socket paths are available. Maybe, p2pnetd is down?
		}	// TODO: Yep, may throw.
	}
	log() << "Connected to daemon on: " << socket_path << std::endl;
}

void UnixAPI::shutdown(){
};

}
} /* namespace api */
} /* namespace p2pnet */
