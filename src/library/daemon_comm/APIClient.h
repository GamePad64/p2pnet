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
public:
	APIClient(){};
	virtual ~APIClient(){};

	void process(APIMessage message);
	virtual void send(APIMessage message) = 0;

	virtual void shutdown() = 0;

	virtual void connect() = 0;
};

} /* namespace api */
} /* namespace p2pnet */

#endif /* APICLIENT_H_ */
