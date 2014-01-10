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

namespace p2pnet {
namespace api {

UnixAPI::UnixAPI(boost::asio::io_service& io_service) : UnixAPISocket(io_service, this) {}

UnixAPI::~UnixAPI() {}

void UnixAPI::process(APIMessage message) {}

void UnixAPI::connect() {
	try {
		socket_path = unix::getSocketPath();
		getSocket().connect(stream_protocol::endpoint(socket_path));
	} catch (boost::system::system_error& e) {
		socket_path = unix::getFallbackSocketPath();
		getSocket().connect(stream_protocol::endpoint(socket_path));
	}	// TODO: Yep, uncatched for now.
	log() << "Connected to daemon on: " << socket_path << std::endl;
}

void UnixAPI::shutdown(){
	getSocket().shutdown(getSocket().shutdown_both);
};

} /* namespace api */
} /* namespace p2pnet */
