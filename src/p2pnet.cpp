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

#include "p2pnet.h"

namespace p2pnet {
P2PErrorCategory_t P2PErrorCategory;

const char* P2PErrorCategory_t::name() const noexcept {
	return "p2pnet";
}

const char* P2PErrorCategory_t::message_c(int error_value) const {
	switch(error_value) {
		case (int)P2PError::daemon_connection_problem:		return "Daemon connection problem";
		case (int)P2PError::too_many_nodes:					return "Node limit exceeded";
		case (int)P2PError::too_many_sockets:				return "Socket limit on specific Node exceeded";
		case (int)P2PError::nonexistent_node:				return "Node doesn't exist";
		case (int)P2PError::nonexistent_socket:				return "Socket doesn't exist on specific Node";

		case (int)P2PError::nodemanager_db_non_consistent:	return "NodeManager DB is inconsistent";

		default: 											return "Unknown error";
	}
}

std::string P2PErrorCategory_t::message(int error_value) const {
	auto str_c = message_c(error_value);
	return std::string(str_c);
}

LIBP2PNET_DLL_EXPORTED const char* p2p_getErrorMessage(P2PError context) {
	return P2PErrorCategory.message_c( (int)context );
}

} /* namespace p2pnet */
