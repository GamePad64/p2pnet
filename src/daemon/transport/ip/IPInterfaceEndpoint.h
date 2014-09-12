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

#ifndef UDPTRANSPORTSOCKETENDPOINT_H_
#define UDPTRANSPORTSOCKETENDPOINT_H_

#include "../InterfaceEndpoint.h"

#include <memory>
#include <boost/asio.hpp>

namespace p2pnet {
namespace transport {

using namespace boost::asio::ip;

class IPInterfaceEndpoint : public InterfaceEndpoint {
public:
	typedef uint16_t port_t;
private:
	address ip;
	port_t port = 0;
protected:
	IPInterfaceEndpoint() = delete;
	IPInterfaceEndpoint(const std::string& prefix);
	IPInterfaceEndpoint(address ip, port_t port, const std::string& prefix);
	IPInterfaceEndpoint(const std::string& ip, port_t port, const std::string& prefix);
public:
	virtual ~IPInterfaceEndpoint(){};

	virtual std::shared_ptr<InterfaceEndpoint> clone();

	bool isIPv4() const;
	bool isIPv6() const;

	std::string getIPString() const;
	void setIPString(const std::string& ip);

	address getAsioIP() const;
	void setAsioIP(const address& ip);

	port_t getPort() const;
	void setPort(port_t port);

	virtual std::string toString() const;
	virtual void fromString(const std::string& string);
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* UDPTRANSPORTSOCKETENDPOINT_H_ */
