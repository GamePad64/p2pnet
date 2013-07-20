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

#ifndef PEERPROCESSOR_H_
#define PEERPROCESSOR_H_

#include "../peer/TH.h"
#include "../net/TransportSocketEndpoint.h"
#include "../abstract/Singleton.h"

namespace p2pnet {
namespace messaging {

class PeerProcessor : public abstract::Singleton<PeerProcessor> {
	inline std::string getComponentName(){
		return "PeerProcessor";
	}
public:
	PeerProcessor();
	virtual ~PeerProcessor();

	void processNewPeerConnection(peer::TH th, net::TransportSocketEndpoint endpoint);
};

} /* namespace messaging */
} /* namespace p2pnet */
#endif /* PEERPROCESSOR_H_ */
