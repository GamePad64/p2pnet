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

#include "../../common/crypto/PublicKeyDSA.h"
#include "../../common/crypto/PrivateKeyDSA.h"
#include "TH.h"
#include "../../common/Config.h"
#include "../../common/Loggable.h"

#include "Handshake.pb.h"

#include <boost/asio.hpp>
#include <boost/asio/system_timer.hpp>
#include <boost/signals2.hpp>
#include <boost/optional.hpp>

#include <thread>
#include <deque>

namespace p2pnet {
namespace overlay {

class KeyInfo {
public:
	void fromProtobuf(const protocol::Handshake_SignedHandshake_KeyInfo& key_info_s);
	protocol::Handshake_SignedHandshake_KeyInfo toProtobuf() const;

	crypto::PrivateKeyDSA private_key;
	crypto::PublicKeyDSA public_key;
	TH th;

	std::chrono::system_clock::time_point expiration_time;
	std::chrono::system_clock::time_point lose_time;
};

class Socket;
class KeyProvider : ConfigClient, public Loggable {
private:
	Socket* parent;

	std::deque<KeyInfo> history;
	unsigned int max_history_size;

	boost::asio::system_timer timer;

	std::chrono::seconds expiration_interval;
	std::chrono::seconds lose_interval;

	std::thread generator_thread;
	std::mutex key_mutex;

	void renewKeys();
	void loopGenerate();

	boost::signals2::signal<void(KeyInfo, KeyInfo)> rotation_signal;
public:
	KeyProvider(Socket* parent);
	virtual ~KeyProvider();

	/**
	 * Gets KeyInfo structure for key, pointed by offset.
	 * @param offset Specifies, which key from history we need to get. 0 - current, 1 - previous. Current key is guaranteed to exist, others are not.
	 * @return
	 */
	boost::optional<KeyInfo> getKeyInfo(unsigned int offset = 0) const;
	boost::optional<KeyInfo> getKeyInfo(const TH& recent_th) const;

	decltype(rotation_signal)& getRotationSignal();
};

} /* namespace databases */
} /* namespace p2pnet */
