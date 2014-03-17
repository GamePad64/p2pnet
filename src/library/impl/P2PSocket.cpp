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
#include "P2PSocket.h"

namespace p2pnet {

P2PSocket::P2PSocket() : P2PSocket(*(Singleton<DaemonConnectionImpl>::getInstance())) {}

P2PSocket::P2PSocket(DaemonConnectionImpl& parent_socket_manager) {
	m_parent_socket_manager = *parent_socket_manager;
	m_api_client_ptr = m_parent_socket_manager->getAPIClient();

	api::APIMessage message;


	m_api_client_ptr->send()
}

P2PSocket::P2PSocket(std::string base58_private_key) : P2PSocket() {
	this->bind(base58_private_key);
}

P2PSocket::P2PSocket(std::string base58_private_key, DaemonConnectionImpl& parent_socket_manager) : P2PSocket(parent_socket_manager) {
	this->bind(base58_private_key);
}

P2PSocket::~P2PSocket() {}

void P2PSocket::accept(P2PAssociation& association) {
	m_api_client_ptr->
}

void P2PSocket::connect(P2PAssociation& association, std::string SH) {
}

void P2PSocket::bind(std::string base58_private_key) {
	m_bound_private_key.setAsBase58(base58_private_key);
}

} /* namespace p2pnet */
