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
#ifndef P2PDAEMON_H_
#define P2PDAEMON_H_

#include "../../common/api/APIMessage.pb.h"

namespace p2pnet {

class P2PDaemon {
/*
	std::thread* socket_thread;
	boost::asio::io_service* m_io_service;
	bool m_external_io_service;

	std::map< int, std::shared_ptr< P2PSocket > > m_socket_ids;
*/
public:
	P2PDaemon();
	virtual ~P2PDaemon();

	virtual void send(api::APIMessage data, int& error_code) = 0;
	virtual api::APIMessage receive(int& error_code) = 0;

	virtual int connect() = 0;
	virtual bool is_connected() = 0;
	virtual int disconnect() = 0;
};

} /* namespace p2pnet */

#endif /* P2PDAEMON_H_ */
