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
#include "TransportConnection.h"

namespace p2pnet {
namespace transport {

TransportConnection::TransportConnection(TransportSocketEndpoint endpoint) : m_endpoint(endpoint) {}

TransportConnection::~TransportConnection() {
	// TODO Auto-generated destructor stub
}

void TransportConnection::process(std::string data) {
	bool empty_connection = bool(m_overlayconnection);
	if(empty_connection){
		m_overlayconnection = std::make_shared()
	}else{
		m_overlayconnection.
	}
}

} /* namespace net */
} /* namespace p2pnet */
