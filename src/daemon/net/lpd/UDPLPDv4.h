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

#ifndef UDPLPDV4_H_
#define UDPLPDV4_H_

#include "UDPLPD.h"
#include <string>

namespace p2pnet {
namespace net {
namespace lpd {

class UDPLPDv4: public UDPLPD {
	const unsigned int m_default_timer_seconds = 10;
	std::string m_default_bind_address = "0.0.0.0";
	std::string m_default_target_address = "239.192.152.144";
	unsigned short m_default_target_port = 28915;
public:
	UDPLPDv4(ConfigManager& config);
	virtual ~UDPLPDv4();

	unsigned short getUDPPort();

	virtual void readConfig();
	virtual void initSocket();
	virtual std::string getComponentName(){
		return "UDPLPDv4";
	}
};

} /* namespace lpd */
} /* namespace net */
} /* namespace p2pnet */
#endif /* UDPLPDV4_H_ */
