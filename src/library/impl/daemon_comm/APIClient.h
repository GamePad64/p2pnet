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
#ifndef APICLIENT_H_
#define APICLIENT_H_

#include "../../common/Loggable.h"
#include "../../common/api/APIMessage.pb.h"

namespace p2pnet {
namespace api {

class APIClient : protected Loggable {
protected:
	typedef std::function<void(int)> SendHandler;
	typedef std::function<void(api::APIMessage, int)> ReceiveHandler;
public:
	APIClient(){};
	virtual ~APIClient(){};

	virtual void send(api::APIMessage data, int& error_code) = 0;
	virtual api::APIMessage receive(int& error_code) = 0;

	virtual void asyncSend(api::APIMessage data, SendHandler send_handler) = 0;
	virtual void asyncReceive(ReceiveHandler receive_handler) = 0;

	virtual void connect() = 0;

	virtual std::shared_ptr<impl::ClientDataSocket> createDataSocket(std::string socket_addr) = 0;
};

} /* namespace api */
} /* namespace p2pnet */

#endif /* APICLIENT_H_ */
