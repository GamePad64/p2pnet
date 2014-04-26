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
#ifndef APISESSION_H_
#define APISESSION_H_

#include "../endpoint/EndpointSocket.h"
#include "../../common/api/APIMessage.pb.h"

#include "../../common/Loggable.h"
#include "../../common/crypto/PrivateKeyDSA.h"

#include <boost/noncopyable.hpp>

namespace p2pnet {
namespace api {

class APIServer;

class APISession : public Loggable, public boost::noncopyable {
	std::map<uint32_t, std::shared_ptr<endpoint::EndpointSocket>> endpoints;
	uint32_t next_id;
	uint32_t socket_count;

	std::pair<uint32_t, std::shared_ptr<endpoint::EndpointSocket>> registerNewSocket();
	void unregisterSocket(uint32_t sock_id);

	void bind(uint32_t sock_id, crypto::PrivateKeyDSA private_key);
	void listen(uint32_t sock_id, uint32_t max_connections);
protected:
	APISession(APIServer* parent);

	APIServer* parent_api_server;
public:
	virtual ~APISession();

	virtual void send(APIMessage message) = 0;
	void process(APIMessage message);

	void dropSession();	// Just a caution: deletes this APISession instance;
};

} /* namespace api */
} /* namespace p2pnet */

#endif /* APISESSION_H_ */
