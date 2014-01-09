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
	log() << "New API session started.";
}
APISession::~APISession() {}

void APISession::process(std::string message) {
	APIMessage message_received;
	message_received.ParseFromString(message);

	switch(message_received.type()){
	case APIMessage::GENERATE_PRIVATE_KEY:
		auto privkey = crypto::PrivateKeyDSA::generateNewKey();

		APIMessage message_reply;
		message_reply.set_type(APIMessage::GENERATE_PRIVATE_KEY_CALLBACK);
		message_reply.set_privkey_cert(privkey.toPEM());
		send(message_reply.SerializeAsString());
	}
}

} /* namespace api */
} /* namespace p2pnet */
