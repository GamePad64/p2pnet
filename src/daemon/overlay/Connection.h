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
#pragma once

#include "TH.h"
#include "PayloadParams.h"
#include "Socket.h"

#include "OverlayProtocol.pb.h"

#include "../transport/Connection.h"
#include "../dht/DHTService.h"

#include "../../common/crypto/ECDH.h"
#include "../../common/crypto/AES.h"
#include "../../common/crypto/PublicKeyDSA.h"

#include "../../common/Config.h"


#include <list>
#include <set>

namespace p2pnet {
using namespace protocol;
namespace overlay {

class Connection : Loggable, public std::enable_shared_from_this<Connection>, public ConfigClient {
	friend class Socket;
	std::weak_ptr<Socket> parent_socket;
	// Types
	enum class State {
		CLOSED = 0,
		SEARCHING_TRANSPORT = 1,
		FOUND_TRANSPORT = 2,
		HANDSHAKE_INIT_SENT = 3,
		HANDSHAKE_REPLY_SENT = 4,
		ESTABLISHED = 5,

		LOST = 255
	} state = State::CLOSED;

	// Identity information
	KeyInfo node_info;
	crypto::ECDH session_ecdh_key;
	crypto::AES session_aes_key;

	std::chrono::system_clock::time_point last_activity;

	std::list<std::shared_ptr<transport::Connection>> transport_connections;

	// Connection callbacks and timeouts
	std::list<std::pair<Socket::ConnectCallback, std::chrono::system_clock::time_point>> connect_callbacks;
	boost::asio::system_timer callback_timer;
	void connectTimeout(std::shared_ptr<Connection> connection, Socket::ConnectCallback callback, const boost::system::error_code& ec);

	// Private Getters
	KeyProvider* getKeyProvider(){return parent_socket.lock()->getKeyProvider();}

	void processHandshake(const OverlayMessage_Header& header, const Handshake& handshake);

	// Actions
	void setState(const State& state_to_set);
	void onConnect();
	void onDisconnect();

public:
	Connection(std::weak_ptr<Socket> parent_socket, TH th);
	virtual ~Connection();

	// Senders
	void send(const OverlayMessage& message, Socket::SendCallback callback);
	void send(const OverlayMessage_Payload& payload,
			Socket::SendCallback callback,
			PayloadPriority prio = PayloadPriority::RELIABLE,
			bool encrypted = true,
			bool force_non_connected = false);

	void process(std::shared_ptr<transport::Connection> origin, const OverlayMessage_Header& header, const OverlayMessage_Body& body);

	// Public Actions
	void connect(Socket::ConnectCallback callback);
	void disconnect();

	// Public Generators
	OverlayMessage genMessageSkel(const TH& from, const TH& to);
	OverlayMessage genMessageSkel();

	// Public getters
	KeyInfo localKeyInfo(){return *(getKeyProvider()->getKeyInfo());}
	KeyInfo remoteKeyInfo(){return node_info;}

	bool connected() const {return state == State::ESTABLISHED;}

	void updateTransport(std::shared_ptr<transport::Connection> tconn, bool verified = false);
	void updateExpirationTime(std::chrono::system_clock::time_point expiry_time);
	void updateLoseTime(std::chrono::system_clock::time_point lost_time);
};

} /* namespace overlay */
} /* namespace p2pnet */
