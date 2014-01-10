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
#ifndef LOCALENDPOINT_H_
#define LOCALENDPOINT_H_

#include "../../common/Config.h"
#include "SH.h"

namespace p2pnet {
namespace endpoint {

class LocalEndpoint : ConfigClient {
	SH endpoint_sh;
public:
	LocalEndpoint();
	virtual ~LocalEndpoint();

	virtual void send(SH dest, std::string data) = 0;
	virtual void process(SH from, std::string data) = 0;

	bool trySendLoopback(SH dest, std::string data);

	SH getSH(){return endpoint_sh;};
};

} /* namespace endpoint */
} /* namespace p2pnet */

#endif /* LOCALENDPOINT_H_ */
