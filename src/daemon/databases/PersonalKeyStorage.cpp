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

#include "PersonalKeyStorage.h"
#include "../../common/crypto/Hash.h"
#include "../AsioIOService.h"
#include <string>
#include <iostream>

namespace p2pnet {
namespace databases {

PersonalKeyStorageClient::PersonalKeyStorageClient() {
	storage = PersonalKeyStorage::getInstance();
	storage->registerClient(this);
}

PersonalKeyStorageClient::~PersonalKeyStorageClient() {
	storage->unregisterClient(this);
}

overlay::TH PersonalKeyStorageClient::getMyTransportHash() {
	return storage->getMyTransportHash();
}

crypto::PublicKeyDSA PersonalKeyStorageClient::getMyPublicKey() {
	return storage->getMyPublicKey();
}

crypto::PrivateKeyDSA PersonalKeyStorageClient::getMyPrivateKey() {
	return storage->getMyPrivateKey();
}

std::shared_ptr<crypto::PrivateKeyDSA> PersonalKeyStorageClient::getPrivateKeyOfTH(overlay::TH hash) {
	return storage->getPrivateKeyOfTH(hash);
}

PersonalKeyStorage::PersonalKeyStorage() : timer(AsioIOService::getIOService()) {
	/*
	 * TODO: We shouldn't regenerate keys every execution, so some sort of caching is required.
	 * Caching will enable our program to start up faster.
	 */
	loopGenerate();
	generator_thread.join();
}
PersonalKeyStorage::~PersonalKeyStorage() {
	timer.cancel();
	if(generator_thread.joinable()){
		generator_thread.join();
	}
}

void PersonalKeyStorage::renewKeys() {
	// Generating all this. This should be asynchronous.
	auto my_new_private_key = std::make_shared<crypto::PrivateKeyDSA>(crypto::PrivateKeyDSA::generateNewKey());
	auto my_new_transport_hash = std::make_shared<crypto::Hash>(crypto::Hash::computeNew(my_new_private_key->derivePublicKey().toBinaryString()));
	auto my_id = std::make_pair(my_new_private_key, my_new_transport_hash);

	// LOCK! Moving faster now!
	//key_lock.lock();
	// Synchronous zone. Now we freeze all the system.
	my_id_history.push_front(my_id);
	// UNLOCK! It's safe now.
	//key_lock.unlock();

	if(my_id_history.size() > getValue<unsigned int>("databases.pks.history_size") + 1 ){	//Yes, +1 means newly generated key.
		my_id_history.pop_back();
	}

	log() << "New keys generated. TH: " << my_id_history.front().second->toBase58() << std::endl;
}

void PersonalKeyStorage::loopGenerate(){
	if(generator_thread.joinable()){
		generator_thread.join();
	}
	generator_thread = std::thread(&PersonalKeyStorage::renewKeys, this);

	timer.expires_from_now(boost::posix_time::minutes(getValue<unsigned int>("databases.pks.renew_interval")));
	timer.async_wait(boost::bind(&PersonalKeyStorage::loopGenerate, this));
}

overlay::TH PersonalKeyStorage::getMyTransportHash() {
	//key_lock.lock();
	auto ret = crypto::Hash(*(my_id_history.front().second));
	//key_lock.unlock();
	return ret;
}

crypto::PublicKeyDSA PersonalKeyStorage::getMyPublicKey() {
	//key_lock.lock();
	auto ret = my_id_history.front().first->derivePublicKey();
	//key_lock.unlock();
	return ret;
}

crypto::PrivateKeyDSA PersonalKeyStorage::getMyPrivateKey() {
	//key_lock.lock();
	auto ret = crypto::PrivateKeyDSA(*(my_id_history.front().first));
	//key_lock.unlock();
	return ret;
}

std::shared_ptr<crypto::PrivateKeyDSA> PersonalKeyStorage::getPrivateKeyOfTH(overlay::TH th){
	//key_lock.lock();
	auto it = std::find_if(
			my_id_history.begin(),
			my_id_history.end(),
			[&] (const std::pair<std::shared_ptr<crypto::PrivateKeyDSA>, std::shared_ptr<overlay::TH>> id_pair) {
		return *(id_pair.second) == th;
	});
	if(it == my_id_history.end()){
		return nullptr;
	}
	auto private_key_ptr = std::make_shared<crypto::PrivateKeyDSA>(*((*it).first));	// Create a copy and pass by shared_ptr.
	//key_lock.unlock();
	return private_key_ptr;
}

void PersonalKeyStorage::registerClient(PersonalKeyStorageClient* client) {
	clients.insert(client);
}

void PersonalKeyStorage::unregisterClient(PersonalKeyStorageClient* client) {
	clients.erase(client);
}

} /* namespace databases */
} /* namespace p2pnet */
