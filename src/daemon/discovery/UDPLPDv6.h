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
namespace discovery {

class UDPLPDv6: public UDPLPD {
public:
	UDPLPDv6(ConfigManager& config);
	virtual ~UDPLPDv6();

	virtual void readConfig();
	virtual std::string getComponentName(){
		return "UDPLPDv6";
	}
};

} /* namespace discovery */
} /* namespace p2pnet */
#endif /* UDPLPDV6_H_ */
