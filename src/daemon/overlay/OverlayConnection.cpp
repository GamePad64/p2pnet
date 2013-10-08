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
#include "OverlayConnection.h"

namespace p2pnet {
namespace overlay {

OverlayConnection::OverlayConnection(overlay::TH th) : th_endpoint(th) {}
OverlayConnection::~OverlayConnection() {}

void OverlayConnection::send(std::string data) {
}

void OverlayConnection::process(std::string data, transport::TransportSocketEndpoint from) {
}

} /* namespace overlay */
} /* namespace p2pnet */
