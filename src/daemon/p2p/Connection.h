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
#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "SH.h"

namespace p2pnet {
namespace p2p {

class Connection {
protected:
	enum ConnectionStatus {
		NOT_CONNECTED,
		AWAITING_ACCEPT
	} connection_status;

	SH connected_sh;
public:
	Connection();
	virtual ~Connection();

	SH getRemoteSH(){return connected_sh;};
};

} /* namespace p2p */
} /* namespace p2pnet */

#endif /* CONNECTION_H_ */
