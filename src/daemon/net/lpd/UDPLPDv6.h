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

#ifndef UDPLPDV6_H_
#define UDPLPDV6_H_

#include "UDPLPD.h"
#include <string>

namespace p2pnet {
namespace net {
namespace lpd {

class UDPLPDv6: public p2pnet::net::lpd::UDPLPD {
	const unsigned int m_default_timer_seconds = 10;
	std::string m_default_bind_address = "0::0";
	std::string m_default_target_address = "ff08::BD02";
	unsigned short m_default_target_port = 28915;
public:
	UDPLPDv6(Config& config, net::UDPTransportSocket& udp_socket);
	virtual ~UDPLPDv6();

	unsigned short getUDPPort();

	virtual void readConfig();
	virtual void initSocket();
	virtual std::string getComponentName(){
		return "UDPLPDv6";
	}
};

} /* namespace lpd */
} /* namespace net */
} /* namespace p2pnet */
#endif /* UDPLPDV6_H_ */
