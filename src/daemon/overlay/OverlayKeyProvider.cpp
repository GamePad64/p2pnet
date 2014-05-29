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

#include "OverlayKeyProvider.h"
#include "OverlaySocket.h"
#include "../../common/crypto/Hash.h"
#include "../AsioIOService.h"

#include <boost/optional.hpp>
#include <boost/date_time.hpp>

#include <mutex>
#include <string>
#include <iostream>

namespace p2pnet {
namespace overlay {

OverlayKeyProvider::OverlayKeyProvider(OverlaySocket* parent) :
		max_history_size(getValue<unsigned int>("overlay.key_provider.history_size") + 1), //Yes, +1 means newly generated key.
		timer(AsioIOService::getIOService()),
		expiration_interval(getValue<unsigned int>("overlay.key_provider.renew_interval")),
		lose_interval(getValue<unsigned int>("overlay.key_provider.renew_interval") * max_history_size) {
	/*
	 * TODO: We shouldn't regenerate keys every execution, so some sort of caching is required.
	 * Caching will enable our program to start up faster.
	 */
	//key_mutex.lock();

	this->parent = parent;
	loopGenerate();
	generator_thread.join();
}
OverlayKeyProvider::~OverlayKeyProvider() {
	timer.cancel();
	if(generator_thread.joinable()){
		generator_thread.join();
	}
}

void OverlayKeyProvider::renewKeys() {
	auto new_private_key = crypto::PrivateKeyDSA::generateNewKey();
	auto new_transport_hash = crypto::Hash(new_private_key);
	auto history_entry = std::make_pair(new_private_key, new_transport_hash);

	if(!history.empty()){
		// Notify OverlaySocket about key rotation, so it could notify connections, recompute DHT K-buckets, do stuff...
		AsioIOService::getIOService().dispatch(std::bind(&OverlaySocket::notifyKeysUpdated, parent, history.front(), history_entry));
	}

	history.push_front(history_entry);
	if(history.size() > max_history_size){
		history.pop_back();
	}

	log() << "New keys generated. TH: " << history.front().second.toBase58() << std::endl;
}

void OverlayKeyProvider::loopGenerate(){
	if(generator_thread.joinable()){
		generator_thread.join();
	}

	auto now = boost::posix_time::second_clock::universal_time();

	expiration_time = now+expiration_interval;
	lose_time = now+lose_interval;

	generator_thread = std::thread(&OverlayKeyProvider::renewKeys, this);

	timer.expires_at(expiration_time);
	timer.async_wait(boost::bind(&OverlayKeyProvider::loopGenerate, this));
}

void OverlayKeyProvider::generatedCallback(std::pair<crypto::PrivateKeyDSA, TH> old_keys, std::pair<crypto::PrivateKeyDSA, TH> new_keys){

}

overlay::TH OverlayKeyProvider::getTH() {
	//std::lock_guard<std::mutex> lock(key_mutex);
	return history.front().second;
}

crypto::PublicKeyDSA OverlayKeyProvider::getPublicKey() {
	//std::lock_guard<std::mutex> lock(key_mutex);
	return history.front().first.derivePublicKey();
}

crypto::PrivateKeyDSA OverlayKeyProvider::getPrivateKey() {
	//std::lock_guard<std::mutex> lock(key_mutex);
	return history.front().first;
}

boost::optional<crypto::PrivateKeyDSA> OverlayKeyProvider::getPrivateKey(overlay::TH th){
	//std::lock_guard<std::mutex> lock(key_mutex);
	auto it = std::find_if(history.begin(), history.end(),
			[&] (const std::pair<crypto::PrivateKeyDSA, overlay::TH> id_pair) {
		return id_pair.second == th;
	});
	if(it == history.end()){
		return boost::none;
	}else{
		return it->first;
	}
}

boost::optional<crypto::PrivateKeyDSA> OverlayKeyProvider::getPrivateKey(std::string binary_th){
	return getPrivateKey(TH::fromBinaryString(binary_th));
}

boost::posix_time::ptime OverlayKeyProvider::getExpirationTime(){
	return expiration_time;
}

std::string OverlayKeyProvider::getExpirationISOTime(){
	return boost::posix_time::to_iso_string(expiration_time);
}
boost::posix_time::ptime OverlayKeyProvider::getLoseTime(){
	return lose_time;
}
std::string OverlayKeyProvider::getLoseISOTime(){
	return boost::posix_time::to_iso_string(lose_time);
}

} /* namespace databases */
} /* namespace p2pnet */
