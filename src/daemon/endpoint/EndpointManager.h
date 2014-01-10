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
#ifndef ENDPOINTMANAGER_H_
#define ENDPOINTMANAGER_H_

#include "../../common/Singleton.h"
#include "../../common/Loggable.h"
#include "SH.h"

#include <memory>
#include <map>

namespace p2pnet {
namespace endpoint {

class LocalEndpoint;

class EndpointManager : public Singleton<EndpointManager>, Loggable {
	std::map<SH, std::shared_ptr<LocalEndpoint>> sh_all_endpoints;
public:
	EndpointManager();
	virtual ~EndpointManager();

	std::shared_ptr<LocalEndpoint> getEndpointPtrBySH(SH sh);
};

} /* namespace endpoint */
} /* namespace p2pnet */

#endif /* ENDPOINTMANAGER_H_ */
