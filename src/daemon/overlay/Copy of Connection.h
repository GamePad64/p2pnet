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
#include "PayloadTypes.h"

#include "../transport/TransportConnection.h"
#include "../../common/crypto/ECDH.h"
#include "../../common/crypto/AES.h"
#include "../../common/crypto/PublicKeyDSA.h"
#include "Protocol.pb.h"
#include "Handshake.pb.h"
#include "../../common/Config.h"
#include "../dht/DHTService.h"

#include <boost/optional.hpp>
#include <deque>
#include <unordered_map>

using std::chrono::system_clock;

namespace p2pnet {
using namespace protocol;
namespace overlay {

class Connection : Loggable, std::enable_shared_from_this<Connection>, public ConfigClient {
	friend class Socket;

	std::shared_ptr<Node> node_ptr;
	Socket* socket_ptr;

	// Types
	enum class State {
		CLOSED = 0,
		PUBKEY_SENT = 1,
		PUBKEY_RECEIVED = 2,
		ECDH_SENT = 3,
		ESTABLISHED = 4,

		LOST = 255
	} state = State::CLOSED;
	enum class Stage {
		PUBKEY = Handshake_Stage_PUBKEY,
		PUBKEY_ACK = Handshake_Stage_PUBKEY_ACK,
		ECDH = Handshake_Stage_ECDH,
		ECDH_ACK = Handshake_Stage_ECDH_ACK,
	};
	typedef Socket::Priority Priority;

	// Variables
	crypto::ECDH session_ecdh_key;
	crypto::AES session_aes_key;

	uint32_t seq_counter = 0;

	/**
	 * This variable holds messages, that were suspended inside sendBinaryData() function
	 * They can be suspended because all TransportSockets are inactive.
	 * They will be sent after discovery new route to TH.
	 */
	std::deque<OverlayMessage> suspended_messages;
	/**
	 * This variable holds messages, that were suspended inside send() function
	 * They can be suspended because AES encryption is not set up yet.
	 * They will be sent after handshake.
	 */
	std::deque<std::pair<OverlayMessage_Payload, Priority>> suspended_payloads;
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

	// Actions
	void performRemoteKeyRotation(std::pair<crypto::PrivateKeyDSA, TH> previous_keys);

	void setState(const State& state_to_set);

	/**
	 * It is a function, that tries to manage TransportConnection directly.
	 * This function is about connectivity, and "send" is about encryption.
	 * @param data
	 */
	void sendMessage(OverlayMessage send_message);

	// Generators
	OverlayMessage genMessageSkel(const TH& src, const TH& dest, Priority prio = Priority::RELIABLE);
	OverlayMessage genMessageSkel(const OverlayMessage_Header& reply_to, Priority prio = Priority::RELIABLE);

	Handshake_PubkeyStage genPubkeyStage(Handshake::Stage for_stage, boost::optional<const crypto::PrivateKeyDSA&> our_hist_key = boost::none);
	Handshake_ECDHStage genECDHStage();

	OverlayMessage& addPayload(const std::string& serialized_payload, PayloadType type, OverlayMessage& message);

	// Processors
	std::map<PayloadType, std::function<void(const OverlayMessage_Header&, std::string)>> processors;

	void processHandshake(const OverlayMessage_Header& header, std::string serialized_payload);

	void processPubkeyStage(const OverlayMessage_Header& header, Handshake handshake_payload);
	void processECDHStage(const OverlayMessage_Header& header, Handshake handshake_payload);
public:
	Connection(Socket* parent_socket, std::shared_ptr<Node> node);
	virtual ~Connection();

	bool connected() const;

	void send(const OverlayMessage_Payload& send_payload, Priority prio);
	void process(const OverlayMessage& recv_message, const transport::SocketEndpoint& from);

	void connect();
	void disconnect();

	// Public getters
	KeyInfo getLocalKeyInfo(){return *(socket_ptr->getKeyProvider()->getKeyInfo());}

	TH localTH(){return socket_ptr->getKeyProvider()->getKeyInfo()->th;}
	TH remoteTH(){return node_ptr->getHash();}

	crypto::PublicKeyDSA localPublicKey(){return socket_ptr->getKeyProvider()->getKeyInfo()->public_key;}
	crypto::PrivateKeyDSA localPrivateKey(){return socket_ptr->getKeyProvider()->getKeyInfo()->private_key;}

	crypto::PublicKeyDSA remotePublicKey(){return node_ptr->getKeyInfo().public_key;}
};

} /* namespace overlay */
} /* namespace p2pnet */

#endif /* OVERLAYCONNECTION_H_ */
