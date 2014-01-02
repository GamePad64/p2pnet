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
#include "LocalService.h"

namespace p2pnet {
namespace service {

LocalService::LocalService(service_type type) {
	this->type = type;
	this->api_service_ptr = nullptr;
}

LocalService::LocalService(service_type type, api::APIService* api_service_ptr) {
	this->type = type;
	this->api_service_ptr = api_service_ptr;
}

LocalService::~LocalService() {}

SH LocalService::getSH() const {
	return SH(private_key.derivePublicKey());
}

} /* namespace service */
} /* namespace p2pnet */
