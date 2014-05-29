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
#include "../../common/Config.h"

#include <deque>
#include <unordered_map>

// TODO: fetch this thing from configuration
#define KEY_ROTATION_LIMIT 5

namespace p2pnet {
namespace overlay {

class OverlayConnection : public Loggable, public std::enable_shared_from_this<OverlayConnection>, public ConfigClient {
	friend class OverlaySocket;
	// Types
	enum class State {
		CLOSED = 0,
		PUBKEY_SENT = 1,
		PUBKEY_RECEIVED = 2,
		ECDH_SENT = 3,
		ECDH_RECEIVED = 4,
		ESTABLISHED = 5,

		LOST = 255
	} state = State::CLOSED;
	enum class Stage {
		PUBKEY = protocol::OverlayMessage_Payload_ConnectionPart_ConnectionStage_PUBKEY,
		PUBKEY_ACK = protocol::OverlayMessage_Payload_ConnectionPart_ConnectionStage_PUBKEY_ACK,
		ECDH = protocol::OverlayMessage_Payload_ConnectionPart_ConnectionStage_ECDH,
		ECDH_ACK = protocol::OverlayMessage_Payload_ConnectionPart_ConnectionStage_ECDH_ACK,
		AES = protocol::OverlayMessage_Payload_ConnectionPart_ConnectionStage_AES,
	};
	typedef OverlaySocket::Priority Priority;

	// Variables
	TH remote_th;

	crypto::PublicKeyDSA remote_public_key;
	crypto::ECDH session_ecdh_key;
	crypto::AES session_aes_key;

	boost::posix_time::ptime remote_key_expiration_time;
	boost::posix_time::ptime remote_key_lose_time;
	boost::asio::deadline_timer remote_key_lose_timer;

	std::deque<transport::TransportSocketEndpoint> transport_endpoints;

	uint32_t seq_counter = 0;

	/**
	 * This variable holds messages, that were suspended inside sendBinaryData() function
	 * They can be suspended because all TransportSockets are inactive.
	 * They will be sent after discovery new route to TH.
	 */
	std::deque<std::string> suspended_binary_data;
	/**
	 * This variable holds messages, that were suspended inside send() function
	 * They can be suspended because AES encryption is not set up yet.
	 * They will be sent after handshake.
	 */
	std::deque<std::pair<protocol::OverlayMessage_Payload, Priority>> suspended_payloads;
	/**
	 * This variable holds messages, that have been sent lately. Useful for resending messages.
	 */
	std::unordered_map<uint32_t, std::string> sent_message_buffer;
	/**
	 * This variable holds messages, that have been processed by us.
	 * It is intended to avoid double-processing in case of resending already processed messages by remote peer
	 * if our ACK messages were not delivered well.
	 */
	std::set<uint32_t> processed_messages;
	std::set<uint32_t> acked_messages;	// Temporary storage for ACK message numbers.

	/**
	 * This is a part of key rotation spam mechanism. If our keys were changed and we receive message,
	 * that was intended for one of our historic keys, then we add KeyRotationPart to all RELIABLE messages.
	 * And begin to performRemoteKeyRotation every X seconds.
	 */
	bool key_rotation_spam_lock = false;
	boost::asio::deadline_timer key_rotation_spam_timer;
	boost::posix_time::seconds key_rotation_spam_limit;

	// Private Getters
	OverlayKeyProvider* getKeyProvider();

	// Private Setters
	void updateExpirationTime(boost::posix_time::ptime expiration_time);
	void updateLoseTime(boost::posix_time::ptime lost_time);
	/* These functions use time in ISO 8601 format like YYYYMMDDThhmmss. */
	void updateExpirationTime(const std::string& iso_time);
	void updateLoseTime(const std::string& iso_time);

	// Actions
	bool performLocalKeyRotation(const protocol::OverlayMessage& recv_message);
	void performRemoteKeyRotation(std::pair<crypto::PrivateKeyDSA, TH> previous_keys);

	void setState(const State& state_to_set);

	/**
	 * It is a function, that tries to manage TransportConnection directly.
	 * This function is about connectivity, and "send" is about encryption.
	 * @param data
	 */
	void sendBinaryData(std::string data);
	void sendRawMessage(protocol::OverlayMessage send_message);

	// Generators
	protocol::OverlayMessage generateReplySkel(const protocol::OverlayMessage& recv_message);
	protocol::OverlayMessage_Payload_ConnectionPart generateConnectionPart(Stage for_stage);
	protocol::OverlayMessage_Payload_KeyRotationPart generateKeyRotationPart(const crypto::PrivateKeyDSA& our_hist_key);	//TODO: This can be cached really good

	// Processors
	void processConnectionPart(const protocol::OverlayMessage& recv_message,
			const protocol::OverlayMessage_Payload& decrypted_payload = protocol::OverlayMessage_Payload::default_instance());
	void processConnectionPartPUBKEY(const protocol::OverlayMessage& recv_message);
	void processConnectionPartECDH(const protocol::OverlayMessage& recv_message);
	void processConnectionPartAES(const protocol::OverlayMessage& recv_message,
			const protocol::OverlayMessage_Payload& decrypted_payload);

	std::string encryptPayload(const protocol::OverlayMessage_Payload& payload);
public:
	OverlayConnection(const TH& th);
	virtual ~OverlayConnection();

	bool connected() const;

	void send(const protocol::OverlayMessage_Payload& send_payload, Priority prio);
	void process(const protocol::OverlayMessage& recv_message, const transport::TransportSocketEndpoint& from);
	void process(const protocol::ConnectionRequestMessage& recv_message, const transport::TransportSocketEndpoint& from);

	void connect();
	void disconnect();

	// Public getters
	TH localTH();
	const TH& remoteTH();

	crypto::PublicKeyDSA localPublicKey();
	crypto::PrivateKeyDSA localPrivateKey();

	const crypto::PublicKeyDSA& remotePublicKey();

	decltype(transport_endpoints) getTransportEndpoints(){return transport_endpoints;};

	// Public setters
	void updateEndpoint(const transport::TransportSocketEndpoint& endpoint, bool verified = false);
};

} /* namespace overlay */
} /* namespace p2pnet */

#endif /* OVERLAYCONNECTION_H_ */
