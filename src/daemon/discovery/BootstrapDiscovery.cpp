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
#include "../errors/NoInterface.h"
#include "../transport/TransportSocket.h"

#include <iostream>
#include <fstream>

namespace p2pnet {
namespace discovery {

BootstrapDiscovery::BootstrapDiscovery() {}
BootstrapDiscovery::~BootstrapDiscovery() {}

void BootstrapDiscovery::run(){
	std::fstream bootstrapfile(ConfigManager::getInstance()->getDirectory()+getValue<std::string>("discovery.bootstrap.filename"));

	std::string peer_readable_tse;

	while(getline(bootstrapfile, peer_readable_tse)){
		auto transport_socket = transport::TransportSocket::getInstance();
		transport::TransportSocketEndpoint tse;
		try {
			tse.fromReadableString(peer_readable_tse);
			handshake(tse);
		} catch(errors::NoInterface* e){
			log() << e->what();
			delete e;
		}
	}
}

} /* namespace discovery */
} /* namespace p2pnet */
