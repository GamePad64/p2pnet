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

#include "../databases/PersonalKeyStorage.h"
#include "MessageParser.h"

namespace p2pnet {
namespace messaging {

MessageParser::MessageParser() {
	// TODO Auto-generated constructor stub

}

MessageParser::~MessageParser() {
	// TODO Auto-generated destructor stub
}

protocol::p2pMessageHeader MessageParser::generateMessageHeader() const{
	crypto::Hash my_th = databases::PersonalKeyStorage::getInstance()->getMyTransportHash();

	protocol::p2pMessageHeader header;
	header.set_src_th(my_th.toBinaryString());
	return header;
}

protocol::p2pMessageHeader MessageParser::generateMessageHeader(const crypto::Hash dest) const {
	crypto::Hash my_th = databases::PersonalKeyStorage::getInstance()->getMyTransportHash();

	protocol::p2pMessageHeader header;
	header.set_src_th(my_th.toBinaryString());
	header.set_dest_th(dest.toBinaryString());
	return header;
}

//protocol::p2pMessageHeader MessageParser::generateMessageHeader(const crypto::Hash src, const crypto::Hash dest) const {}

protocol::p2pMessage MessageParser::generateAgreementMessage() {
	protocol::p2pMessage message;
	(*message.mutable_message_header()) = generateMessageHeader();

	message.set_message_type(protocol::p2pMessage::AGREEMENT);
	auto agreement = message.mutable_agreement();

	auto pubkey = databases::PersonalKeyStorage::getInstance()->getMyPublicKey();
	agreement->set_src_pubkey(std::string(pubkey.begin(), pubkey.end()));
	return message;
}

} /* namespace messaging */
} /* namespace p2pnet */
