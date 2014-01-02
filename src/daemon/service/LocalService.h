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
#ifndef LOCALSERVICE_H_
#define LOCALSERVICE_H_

#include "../../common/crypto/PrivateKeyDSA.h"
#include "SH.h"

namespace p2pnet {

namespace api {
class APIService;
}

namespace service {

class LocalService {
protected:
	api::APIService* api_service_ptr;

	enum service_type {
		STANDARD = 0,
		//BROADCAST = 1
	} type;

	crypto::PrivateKeyDSA private_key;	//TODO use system-wide cryptographic storage.
	std::set<std::string> namespaces;
public:
	LocalService(service_type type);
	LocalService(service_type type, api::APIService* api_service_ptr);
	virtual ~LocalService();

	SH getSH() const;

	virtual void send(SH dest, std::string data) = 0;
	virtual void process(SH from, std::string data) = 0;
};

} /* namespace service */
} /* namespace p2pnet */

#endif /* LOCALSERVICE_H_ */
