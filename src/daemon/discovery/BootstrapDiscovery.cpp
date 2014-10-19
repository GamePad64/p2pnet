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
#include "BootstrapDiscovery.h"
#include "../transport/Socket.h"

#include <iostream>
#include <fstream>

namespace p2pnet {
namespace discovery {

BootstrapDiscovery::BootstrapDiscovery(std::shared_ptr<transport::Socket> transport_socket, std::shared_ptr<overlay::Socket> overlay_socket) :
		DiscoveryService(transport_socket, overlay_socket) {}
BootstrapDiscovery::~BootstrapDiscovery() {}

void BootstrapDiscovery::run(){
	log() << "Starting bootstrap" << std::endl;
	std::fstream bootstrapfile(ConfigManager::getInstance()->getDirectory()+getValue<std::string>("discovery.bootstrap.filename"));

	std::string peer_readable_tse;

	while(getline(bootstrapfile, peer_readable_tse)){
		transport::SocketEndpoint tse(transport_socket.get());
		try {
			tse.fromString(peer_readable_tse);
			handshake(tse);
		} catch(int e){
			log() << e << std::endl;
		}
	}
}

} /* namespace discovery */
} /* namespace p2pnet */
