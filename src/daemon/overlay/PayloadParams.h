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
#ifndef PAYLOADTYPES_H_
#define PAYLOADTYPES_H_

#include "OverlayProtocol.pb.h"

namespace p2pnet {
namespace overlay {

enum class PayloadType {
	UNKNOWN = 255,

	HANDSHAKE = 1,
	ONION = 2,

	DHT = 3
};

enum class PayloadPriority {
	REALTIME = protocol::OverlayMessage_Header_MessagePriority_REALTIME,
	RELIABLE = protocol::OverlayMessage_Header_MessagePriority_RELIABLE
};

}
}

#endif /* PAYLOADTYPES_H_ */
