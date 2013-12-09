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
#include "../overlay/TH.h"
#include "../../common/Singleton.h"
#include "../../common/Config.h"
#include "../../common/Loggable.h"

#include <boost/asio.hpp>
#include <thread>
#include <deque>

namespace p2pnet {
namespace databases {

typedef std::deque<std::pair<std::shared_ptr<crypto::PrivateKeyDSA>, std::shared_ptr<overlay::TH>>> key_history_t;

class PersonalKeyStorageClient;

class PersonalKeyStorage : public Singleton<PersonalKeyStorage>, ConfigClient, public Loggable {
	key_history_t my_id_history;
	//std::mutex key_lock;

	std::set<PersonalKeyStorageClient*> clients;

	boost::asio::deadline_timer timer;
	boost::posix_time::ptime expiry_time;
	boost::posix_time::ptime lose_time;
	std::thread generator_thread;

	void renewKeys();
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

	const key_history_t& getHistory(){return my_id_history;};

	boost::posix_time::ptime getExpiryTime() const {return expiry_time;};
	boost::posix_time::ptime getLoseTime() const {return lose_time;};

	std::string getComponentName(){return "PersonalKeyStorage";}

	void registerClient(PersonalKeyStorageClient* client);
	void unregisterClient(PersonalKeyStorageClient* client);
};

class PersonalKeyStorageClient {
public:
	PersonalKeyStorageClient();
	virtual ~PersonalKeyStorageClient();

	// Signals
	virtual void keysUpdated(boost::posix_time::ptime expiry_time, boost::posix_time::ptime lose_time){};

protected:
	PersonalKeyStorage* pks;	// Yes, it is protected for "raw" access.
	// Getters
	overlay::TH getMyTransportHash(){return pks->getMyTransportHash();}

	crypto::PublicKeyDSA getMyPublicKey(){return pks->getMyPublicKey();}
	crypto::PrivateKeyDSA getMyPrivateKey(){return pks->getMyPrivateKey();}

	std::shared_ptr<crypto::PrivateKeyDSA> getPrivateKeyOfTH(overlay::TH hash){return pks->getPrivateKeyOfTH(hash);}
	const key_history_t& getHistory(){return pks->getHistory();}

	boost::posix_time::ptime getExpiryTime() const{return pks->getExpiryTime();};
	boost::posix_time::ptime getLoseTime() const{return pks->getLoseTime();};
};

} /* namespace databases */
} /* namespace p2pnet */
#endif /* PERSONALKEYSTORAGE_H_ */
