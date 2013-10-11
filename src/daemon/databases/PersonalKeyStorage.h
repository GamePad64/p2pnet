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

#include "../../common/crypto/PrivateKeyDSA.h"
#include "../overlay/TH.h"
#include "../abstract/Singleton.h"
#include "../../common/Config.h"
#include "../../common/Loggable.h"

#include <boost/asio.hpp>
#include <thread>
#include <deque>

namespace p2pnet {
namespace databases {

class PersonalKeyStorage;
class PersonalKeyStorageClient {
	PersonalKeyStorage* storage;
public:
	PersonalKeyStorageClient();
	~PersonalKeyStorageClient();

	// Signals
	void keysUpdated(){};

protected:
	// Getters
	overlay::TH getMyTransportHash();

	crypto::PublicKeyDSA getMyPublicKey();
	crypto::PrivateKeyDSA getMyPrivateKey();

	std::shared_ptr<crypto::PrivateKeyDSA> getPrivateKeyOfTH(overlay::TH);
};

class PersonalKeyStorage : public abstract::Singleton<PersonalKeyStorage>, public Loggable, public ConfigClient {
	std::deque<std::unique_ptr<crypto::PrivateKeyDSA>> my_private_key_history;
	std::deque<std::unique_ptr<overlay::TH>> my_transport_hash_history;
	std::mutex key_lock;

	boost::asio::deadline_timer timer;

	std::set<PersonalKeyStorageClient*> clients;

	void renewKeys();

	std::thread generator_thread;

	void loopGenerate();
public:
	PersonalKeyStorage();
	virtual ~PersonalKeyStorage();

	/**
	 * This function returns own transport hash from cache. If there is no such hash, then regenerate keys and hashes.
	 * @return My transport hash
	 */
	overlay::TH getMyTransportHash();

	crypto::PublicKeyDSA getMyPublicKey();
	crypto::PrivateKeyDSA getMyPrivateKey();

	std::shared_ptr<crypto::PrivateKeyDSA> getPrivateKeyOfTH(overlay::TH th);

	void registerClient(PersonalKeyStorageClient* client);
	void unregisterClient(PersonalKeyStorageClient* client);
};

} /* namespace databases */
} /* namespace p2pnet */
#endif /* PERSONALKEYSTORAGE_H_ */
