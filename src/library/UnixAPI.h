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
#ifndef UNIXAPI_H_
#define UNIXAPI_H_

#include "../common/Loggable.h"
#include "../common/api/UnixAPISocket.h"
#include "../common/api/LowLevelAPISession.h"

namespace p2pnet {
namespace api {

class UnixAPI : public unix::UnixAPISocket, public LowLevelAPISession, Loggable {
	std::string socket_path;
public:
	UnixAPI(boost::asio::io_service& io_service);
	virtual ~UnixAPI();

	void process(APIMessage message);

	void shutdown();

	void connect();
};

} /* namespace api */
} /* namespace p2pnet */

#endif /* UNIXAPI_H_ */
