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
#include "DHTService.h"

namespace p2pnet {
namespace dht {

DHTClient::DHTClient(const DHTService& parent_service){
	service_ptr = &parent_service;
}

DHTService::DHTService() {}
DHTService::~DHTService() {}

boost::optional<std::string> DHTService::getStoredValue(DHTCoords coords) {
	auto it = stored_values.find(coords);
	if(it != stored_values.end()){
		return boost::optional<std::string>(*it);
	}
	return boost::optional<std::string>();
}

void DHTService::findValue(DHTClient* client, DHTCoords coords, const crypto::Hash& hash){

}
void DHTService::postValue(DHTClient* client, DHTCoords coords, std::string value){

}

/* Listener mgmt */
void DHTService::registerClient(DHTClient* listener_ptr, std::string namespace_hook) {
	dht_clients.insert(std::make_pair(namespace_hook, listener_ptr));
}
void DHTService::unregisterClient(DHTClient* listener_ptr, std::string namespace_hook) {
	auto ns_range = dht_clients.equal_range(namespace_hook);
	for(auto it = ns_range.first; it != ns_range.second; ++it){
		if(*it == listener_ptr){
			dht_clients.erase(it);
			break;
		}
	}
}

} /* namespace dht */
} /* namespace p2pnet */
