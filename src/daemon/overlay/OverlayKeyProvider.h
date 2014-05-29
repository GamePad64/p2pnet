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

#ifndef PERSONALKEYSTORAGE_H_
#define PERSONALKEYSTORAGE_H_

#include "../../common/crypto/PublicKeyDSA.h"
#include "../../common/crypto/PrivateKeyDSA.h"
#include "TH.h"
#include "../../common/Singleton.h"
#include "../../common/Config.h"
#include "../../common/Loggable.h"

#include <boost/asio.hpp>
#include <thread>
#include <deque>

namespace p2pnet {
namespace overlay {

class OverlaySocket;

class OverlayKeyProvider : ConfigClient, public Loggable {
	OverlaySocket* parent;

	std::deque<std::pair<crypto::PrivateKeyDSA, TH>> history;

	unsigned int max_history_size;

	boost::asio::deadline_timer timer;

	boost::posix_time::ptime expiration_time;
	boost::posix_time::ptime lose_time;

	boost::posix_time::minutes expiration_interval;
	boost::posix_time::minutes lose_interval;

	std::thread generator_thread;
	std::mutex key_mutex;

	void renewKeys();
	void loopGenerate();
	void generatedCallback(std::pair<crypto::PrivateKeyDSA, TH> old_keys, std::pair<crypto::PrivateKeyDSA, TH> new_keys);
public:
	OverlayKeyProvider(OverlaySocket* parent);
	virtual ~OverlayKeyProvider();

	/**
	 * This function returns own transport hash from cache. If there is no such hash, then regenerate keys and hashes.
	 * @return My transport hash
	 */
	overlay::TH getTH();

	crypto::PublicKeyDSA getPublicKey();
	crypto::PrivateKeyDSA getPrivateKey();
	boost::optional<crypto::PrivateKeyDSA> getPrivateKey(overlay::TH th);
	boost::optional<crypto::PrivateKeyDSA> getPrivateKey(std::string binary_th);

	boost::optional<crypto::PrivateKeyDSA> getPreviousPrivateKey();

	boost::posix_time::ptime getExpirationTime();
	std::string getExpirationISOTime();
	boost::posix_time::ptime getLoseTime();
	std::string getLoseISOTime();
};

} /* namespace databases */
} /* namespace p2pnet */
#endif /* PERSONALKEYSTORAGE_H_ */
