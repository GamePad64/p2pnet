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
#ifndef SERVICEMANAGER_H_
#define SERVICEMANAGER_H_

#include <map>
#include <list>
#include <memory>

namespace p2pnet {

namespace api {
class APIService;
}

namespace service {

class LocalService;

class ServiceManager {
	std::map<api::APIService*, std::shared_ptr<LocalService>> api_services;
	std::list<std::shared_ptr<LocalService>> system_services;

	std::map<SH, std::shared_ptr<LocalService>> services_sh;
public:
	ServiceManager();
	virtual ~ServiceManager();

	void registerAPIService(api::APIService* api_service);
	void removeAPIService(api::APIService* api_service);
};

} /* namespace service */
} /* namespace p2pnet */

#endif /* SERVICEMANAGER_H_ */
