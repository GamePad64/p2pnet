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
#include "LocalEndpoint.h"
#include "EndpointManager.h"

namespace p2pnet {
namespace endpoint {

LocalEndpoint::LocalEndpoint() {
	// TODO Auto-generated constructor stub

}

LocalEndpoint::~LocalEndpoint() {
	// TODO Auto-generated destructor stub
}

bool LocalEndpoint::trySendLoopback(SH dest, std::string data) {
	auto endpoint_ptr = EndpointManager::getInstance()->getEndpointPtrBySH(dest);
	if(getValue<bool>("endpoint.allow_loopback") && endpoint_ptr){
		endpoint_ptr->process(endpoint_sh, data);
		return true;
	}
	return false;
}

} /* namespace endpoint */
} /* namespace p2pnet */
