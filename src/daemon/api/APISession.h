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

#include "../endpoint/LocalEndpoint.h"
#include "../../common/api/LowLevelAPISession.h"

namespace p2pnet {
namespace api {

class APISession : Loggable, public LowLevelAPISession {
	std::map<uint32_t, std::shared_ptr<endpoint::LocalEndpoint>> endpoints;
public:
	APISession();
	virtual ~APISession();

	virtual void send(APIMessage message) = 0;
	void process(APIMessage message);

	std::string getComponentName(){return "APISession";}
};

} /* namespace api */
} /* namespace p2pnet */

#endif /* APISESSION_H_ */
