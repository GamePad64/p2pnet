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
#include "APIManager.h"

#include "../../common/Config.h"
#include "UnixAPIServer.h"
#include "../AsioIOService.h"

namespace p2pnet {
namespace api {

APIManager::APIManager() {
	if(ConfigManager::getInstance()->getValue<bool>("api.unix.enabled")){
		try {
			auto api_unix = new api::unix::UnixAPIServer(AsioIOService::getIOService());
			server_list.push_back(api_unix);
		} catch (boost::system::system_error& e) {
			log() << "Unable to initialize Unix Socket API. Exception caught: " << e.what() << std::endl;
		}
	}
}

APIManager::~APIManager() {
	for(auto server_ptr : server_list){
		delete server_ptr;
	}
}

} /* namespace api */
} /* namespace p2pnet */
