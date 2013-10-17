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
#include "../../common/crypto/AES.h"
#include "../../common/crypto/PublicKeyDSA.h"
#include "../protobuf/Protocol.pb.h"
#include <deque>
#include <unordered_map>

namespace p2pnet {
namespace overlay {

class OverlayConnection : public Loggable {
	overlay::TH th_endpoint;
	crypto::PublicKeyDSA public_key;

	crypto::ECDH ecdh_key;
	crypto::AES aes_key;

	std::deque<transport::TransportSocketEndpoint> m_tse;
	// This is about messages, that we can't deliver, because all the TransportSockets are inactive.
	std::deque<std::string> suspended_messages;
	std::deque<std::string> suspended_data;	// These messages are not delivered, as we didn't set up encryption.
	std::unordered_map<uint32_t, std::string> sent_message_buffer;
	std::deque<uint32_t> processed_messages;	// To avoid double-processing. If our ACK messages were not delivered well.

	enum States {
		CLOSED = 0,
		PUBKEY_SENT = 1,
		PUBKEY_RECEIVED = 2,
		ECDH_SENT = 3,
		ECDH_RECEIVED = 4,
		ESTABLISHED = 5
	} state = CLOSED;

	databases::PersonalKeyStorage* pks;

	/**
	 * It is a function, that tries to manage TransportConnection directly.
	 * This function is about connectivity, and "send" is about encryption.
	 * @param data
	 */
	void sendRaw(std::string data);

	protocol::OverlayMessageStructure generateReplySkel(const protocol::OverlayMessageStructure& recv_message);
	// This method generates KeyRotation payload using given ECDSA private key.
	void addKeyRotationPart(protocol::OverlayMessageStructure& answ_message,
			bool& send_answ,
			std::shared_ptr<crypto::PrivateKeyDSA> old_dsa_private);
	void addTransmissionControlPart(protocol::OverlayMessageStructure& answ_message,
			bool& send_answ,
			const protocol::OverlayMessageStructure& recv_message);

public:
	OverlayConnection(overlay::TH th);
	virtual ~OverlayConnection();

	bool isReady() const;

	void updateTransportSocketEndpoint(transport::TransportSocketEndpoint from);

	void send(std::string data);
	void process(std::string data, transport::TransportSocketEndpoint from);

	void processConnectionMessage(protocol::OverlayMessageStructure message);
	void processConnectionPUBKEYMessage(protocol::OverlayMessageStructure message);
	void processConnectionECDHMessage(protocol::OverlayMessageStructure message);
	void processConnectionACKMessage(protocol::OverlayMessageStructure message);

	std::string getComponentName(){return "OverlayConnection";}
};

} /* namespace overlay */
} /* namespace p2pnet */

#endif /* OVERLAYCONNECTION_H_ */
