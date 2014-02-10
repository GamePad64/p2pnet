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
#ifndef DAEMONCONNECTION_H_
#define DAEMONCONNECTION_H_

#ifdef __cplusplus
/* C++ Class Declaration */

#ifdef ENABLE_ASIO
#include <boost/asio.hpp>
#endif

namespace p2pnet {

class DaemonConnection {
	class DaemonConnectionImpl;
	DaemonConnectionImpl* impl;
public:
	DaemonConnection();
#ifdef ENABLE_ASIO
	DaemonConnection(boost::asio::io_service& io_service);
#endif
	virtual ~DaemonConnection();
};

} /* namespace p2pnet */
typedef p2pnet::DaemonConnection p2pnet_DaemonConnection;
/* End Of C++ Class Declaration */
#else
/* C Struct Declaration */
	struct p2pnet_DaemonConnection;
/* End Of C Struct Declaration */
#endif

#ifdef __cplusplus
extern "C" {
#endif
/* C API Declaration */
p2pnet_DaemonConnection* p2pnet_connectDaemon();
void p2pnet_disconnectDaemon(p2pnet_DaemonConnection* connection);
/* End Of C API Declaration */
#ifdef __cplusplus
}
#endif

#endif /* DAEMONCONNECTION_H_ */
