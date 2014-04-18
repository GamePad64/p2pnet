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
#ifndef APISERVER_H_
#define APISERVER_H_

#include <memory>
#include <set>
#include <boost/noncopyable.hpp>

namespace p2pnet {
namespace api {

class APISession;

class APIServer : public boost::noncopyable, std::enable_shared_from_this<APIServer> {
protected:
	std::set<std::unique_ptr<APISession>> api_sessions;
public:
	APIServer();
	virtual ~APIServer();

	virtual void accept() = 0;

	void dropSession(APISession* session_ptr);
};

} /* namespace api */
} /* namespace p2pnet */

#endif /* APISERVER_H_ */
