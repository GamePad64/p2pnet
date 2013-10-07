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

#ifndef GENERICLPD_H_
#define GENERICLPD_H_

#include "../../common/Config.h"

namespace p2pnet {
namespace discovery {

class GenericLPD : public ConfigClient {
public:
	GenericLPD(ConfigManager& parent_config);
	virtual ~GenericLPD();


};

} /* namespace discovery */
} /* namespace p2pnet */
#endif /* GENERICLPD_H_ */
