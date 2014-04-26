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
#ifndef ENDPOINTSOCKET_H_
#define ENDPOINTSOCKET_H_

#include "SH.h"
#include "../../common/crypto/PublicKeyDSA.h"
#include "../../common/crypto/PrivateKeyDSA.h"
#include "../../common/Loggable.h"
#include "../../common/Config.h"

#include "GenericDestination.h"

#include <memory>
#include <deque>

namespace p2pnet {
namespace api{class APISession;};
namespace endpoint {

class EndpointSocket : public std::enable_shared_from_this<EndpointSocket>, public GenericDestination, Loggable, ConfigClient {
	crypto::PrivateKeyDSA bound_privkey;
	api::APISession* parent_api_session;

	int listen_max_conn = 0;
	std::deque<GenericDestination*> accept_queue;
	GenericDestination* connected_endpoint;
public:
	EndpointSocket(api::APISession* parent);
	virtual ~EndpointSocket();

	void bind(crypto::PrivateKeyDSA privkey);
	void listen(int max_conn);

	void connect(SH destination_sh);
	void accept();

	SH getBoundSH();

	virtual void handleConnectRequest(std::shared_ptr<GenericDestination> endpoint);
};

} /* namespace endpoint */
} /* namespace p2pnet */

#endif /* ENDPOINTSOCKET_H_ */
