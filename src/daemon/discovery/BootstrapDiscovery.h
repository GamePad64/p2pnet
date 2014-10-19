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
#ifndef BOOTSTRAPDISCOVERY_H_
#define BOOTSTRAPDISCOVERY_H_

#include "DiscoveryService.h"

namespace p2pnet {
namespace discovery {

class BootstrapDiscovery : public DiscoveryService {
public:
	BootstrapDiscovery(std::shared_ptr<transport::Socket> transport_socket, std::shared_ptr<overlay::Socket> overlay_socket);
	virtual ~BootstrapDiscovery();

	void run();
};

} /* namespace discovery */
} /* namespace p2pnet */

#endif /* BOOTSTRAPDISCOVERY_H_ */
