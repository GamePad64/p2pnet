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
#include "../databases/PersonalKeyStorage.h"
#include "../../common/Config.h"
#include "OverlayPeer.h"
#include <deque>
#include <unordered_map>

// TODO: fetch this thing from configuration
#define KEY_ROTATION_LIMIT 5

namespace p2pnet {
namespace overlay {

class OverlayConnection : public Loggable, public std::enable_shared_from_this<OverlayConnection>, public ConfigClient, databases::PersonalKeyStorageClient {
	std::shared_ptr<OverlayPeer> overlay_peer_ptr;

	virtual void keysUpdated();

	uint32_t seq_counter = 0;

	std::deque<transport::TransportSocketEndpoint> m_tse;
	// This is about messages, that we can't deliver, because all the TransportSockets are inactive.
	std::deque<std::string> suspended_data;
	std::deque<std::pair<protocol::OverlayMessage_Payload, protocol::OverlayMessage_Header_MessagePriority>> suspended_payloads;	// These messages are not delivered, as we didn't set up encryption.
	std::unordered_map<uint32_t, std::string> sent_message_buffer;
	std::set<uint32_t> processed_messages;	// To avoid double-processing. If our ACK messages were not delivered well.
	std::set<uint32_t> acked_messages;	// Temporary storage for ACK message numbers.

	boost::asio::deadline_timer udp_key_rotation_limit;
	bool udp_key_rotation_locked = false;

	enum States {
		CLOSED = 0,
		PUBKEY_SENT = 1,
		PUBKEY_RECEIVED = 2,
		ECDH_SENT = 3,
		ECDH_RECEIVED = 4,
		ESTABLISHED = 5
	} state = CLOSED;
	void setState(const States& state_to_set);

	/**
	 * It is a function, that tries to manage TransportConnection directly.
	 * This function is about connectivity, and "send" is about encryption.
	 * @param data
	 */
	void sendData(std::string data);
	void sendMessage(const protocol::OverlayMessage& send_message);

	bool performLocalKeyRotation(const protocol::OverlayMessage& recv_message);

	// Generators
	protocol::OverlayMessage generateReplySkel(const protocol::OverlayMessage& recv_message);
	protocol::OverlayMessage_Payload_ConnectionPart generateConnectionPart(protocol::OverlayMessage_Payload_ConnectionPart_ConnectionStage for_stage);
	protocol::OverlayMessage_Payload_KeyRotationPart generateKeyRotationPart(const crypto::PrivateKeyDSA& our_hist_key);

	// Processors
	void processConnectionPart(const protocol::OverlayMessage& recv_message,
			const protocol::OverlayMessage_Payload& decrypted_payload = protocol::OverlayMessage_Payload::default_instance());
	void processConnectionPartPUBKEY(const protocol::OverlayMessage& recv_message);
	void processConnectionPartECDH(const protocol::OverlayMessage& recv_message);
	void processConnectionPartAES(const protocol::OverlayMessage& recv_message,
			const protocol::OverlayMessage_Payload& decrypted_payload);

public:
	OverlayConnection(std::shared_ptr<OverlayPeer> overlay_peer);
	virtual ~OverlayConnection();

	bool isReady() const;
	void updateTSE(const transport::TransportSocketEndpoint& from, bool verified = false);

	std::string encryptPayload(const protocol::OverlayMessage_Payload& payload);

	void send(const protocol::OverlayMessage_Payload& send_payload, const protocol::OverlayMessage_Header_MessagePriority& prio);
	void process(const protocol::OverlayMessage& recv_message, const transport::TransportSocketEndpoint& from);
	void process(const protocol::ConnectionRequestMessage& recv_message, const transport::TransportSocketEndpoint& from);

	void connect();

	std::string getComponentName(){return "OverlayConnection";}
};

} /* namespace overlay */
} /* namespace p2pnet */

#endif /* OVERLAYCONNECTION_H_ */
