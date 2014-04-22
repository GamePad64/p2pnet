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

#include "../../common/crypto/PrivateKeyDSA.h"
#include "../../common/crypto/PublicKeyDSA.h"
#include "../../common/crypto/Hash.h"

#include "../p2pnet.h"

namespace p2pnet {

LIBP2PNET_DLL_EXPORTED void generateKeyPair(std::string& private_key_b58, std::string& public_key_b58){
	auto new_private_key = crypto::PrivateKeyDSA::generateNewKey();
	auto new_public_key = new_private_key.derivePublicKey();

	private_key_b58 = new_private_key.toBase58();
	public_key_b58 = new_public_key.toBase58();
}

LIBP2PNET_DLL_EXPORTED void p2p_generateKeyPair(char*& private_key_b58, char*& public_key_b58){
	std::string private_key_b58_s, public_key_b58_s;
	generateKeyPair(private_key_b58_s, public_key_b58_s);

	private_key_b58 = new char[private_key_b58_s.size()+1];
	public_key_b58 = new char[public_key_b58_s.size()+1];

	strcpy(private_key_b58, private_key_b58_s.c_str());
	strcpy(public_key_b58, public_key_b58_s.c_str());
}

} /* namespace p2pnet */
