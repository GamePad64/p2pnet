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
#ifndef APIPROCESSOR_H_
#define APIPROCESSOR_H_

#include "APIMessage.pb.h"

namespace p2pnet {
namespace api {

/**
 * APIProcessor is an interface that is used to have just `process(APIMessage message)` fuinction.
 * Its meaning is 'something that could process API requests'.
 */
class LowLevelAPISession {
public:
	virtual ~LowLevelAPISession(){};

	virtual void process(APIMessage message) = 0;
	virtual void shutdown() = 0;	// This signal is generated if remote endpoint is disconnected.
};

} /* namespace api */
} /* namespace p2pnet */

#endif /* APIPROCESSOR_H_ */
