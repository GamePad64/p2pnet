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
#include "APISession.h"
#include "../../common/crypto/PrivateKeyDSA.h"
#include "../../common/api/APIMessage.pb.h"

namespace p2pnet {
namespace api {

APISession::APISession() {
	log() << "New API session started" << std::endl;
}
APISession::~APISession() {
	log() << "API session shut down" << std::endl;
}

void APISession::process(APIMessage message) {
	switch(message.type()){
	case APIMessage::GENERATE_PRIVATE_KEY:
		auto privkey = crypto::PrivateKeyDSA::generateNewKey();

		APIMessage message_reply;
		message_reply.set_type(APIMessage::GENERATE_PRIVATE_KEY_CALLBACK);
		message_reply.set_privkey_cert(privkey.toHex());
		send(message_reply);
	}
}

} /* namespace api */
} /* namespace p2pnet */
