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

#include "KeyProvider.h"
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

KeyProvider::KeyProvider(Socket* parent) :
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
KeyProvider::~KeyProvider() {
	timer.cancel();
	if(generator_thread.joinable()){
		generator_thread.join();
	}
}

void KeyProvider::renewKeys() {
	auto now = std::chrono::system_clock::now();

	KeyInfo new_info;
	new_info.expiration_time = now+expiration_interval;
	new_info.lose_time = now+lose_interval;
	new_info.private_key = crypto::PrivateKeyDSA::generateNewKey();
	new_info.public_key = new_info.private_key.derivePublicKey();
	new_info.th = crypto::Hash(new_info.private_key);

	if(!history.empty()){
		// Notify OverlaySocket about key rotation, so it could notify connections, recompute DHT K-buckets, do stuff...
		AsioIOService::getIOService().dispatch([&](){
			rotation_signal(history.front(), new_info);}
		);
	}

	history.push_front(new_info);
	if(history.size() > max_history_size){
		history.pop_back();
	}

	timer.expires_at(new_info.expiration_time);
	timer.async_wait(boost::bind(&KeyProvider::loopGenerate, this));

	log() << "New keys generated. TH: " << history.front().th.toBase58() << std::endl;
}

void KeyProvider::loopGenerate(){
	if(generator_thread.joinable()){
		generator_thread.join();
	}

	generator_thread = std::thread(&KeyProvider::renewKeys, this);
}

boost::optional<KeyInfo> KeyProvider::getKeyInfo(unsigned int offset) const {
	try {
		return history.at(offset);
	}catch(const std::out_of_range&) {	// This is really normal case
		return boost::none;
	}
}

boost::optional<KeyInfo> KeyProvider::getKeyInfo(const TH& recent_th) const {
	//std::lock_guard<std::mutex> lock(key_mutex);
	auto it = std::find_if(history.begin(), history.end(), [&](const KeyInfo& id_pair) {return id_pair.th == recent_th;});
	if(it == history.end()){
		return boost::none;
	}else{
		return *it;
	}
}

} /* namespace databases */
} /* namespace p2pnet */
