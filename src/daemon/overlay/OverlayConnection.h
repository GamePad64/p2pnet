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
#ifndef OVERLAYCONNECTION_H_
#define OVERLAYCONNECTION_H_

#include "TH.h"
#include "../transport/TransportConnection.h"
#include "../../common/crypto/ECDH.h"
#include "../../common/crypto/PublicKeyDSA.h"
#include "../protobuf/Protocol.pb.h"
#include <deque>

namespace p2pnet {
namespace overlay {

class OverlayConnection : public Loggable {
	overlay::TH th_endpoint;
	crypto::PublicKeyDSA public_key;

	crypto::ECDH ecdh_key;

	std::deque<transport::TransportSocketEndpoint> m_tse;
	std::deque<std::string> suspended_raw_messages;
	std::deque<std::string> suspended_encrypted_messages;

	enum States {
		CLOSED,
		PUBKEY_SENT,
		PUBKEY_RECEIVED,
		ECDH_SENT,
		ECDH_RECEIVED,
		ESTABLISHED
	} state = CLOSED;

	/**
	 * It is a function, that tries to manage TransportConnection directly.
	 * This function is about connectivity, and "send" is about encryption.
	 * @param data
	 */
	void sendRaw(std::string data);

public:
	OverlayConnection(overlay::TH th);
	virtual ~OverlayConnection();

	bool isReady() const;

	void send(std::string data);
	void process(std::string data, transport::TransportSocketEndpoint from);

	void processConnectionMessage(protocol::OverlayMessageStructure message);

	std::string getComponentName(){return "OverlayConnection";}
};

} /* namespace overlay */
} /* namespace p2pnet */

#endif /* OVERLAYCONNECTION_H_ */
