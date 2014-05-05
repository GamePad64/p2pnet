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
#include "P2PDaemon.h"

namespace p2pnet {

P2PDaemon::P2PDaemon() {}

void P2PDaemon::handleSend(uint32_t message_token, std::error_condition& error) {
	awaiting_receive[message_token];
}

void P2PDaemon::handleReceive(api::APIMessage message, std::error_condition& error) {
	auto token = message.message_token();

	auto promise_it = awaiting_receive.find(token);
	promise_it->second.set_value(message);

	// Continue receive loop
	startReceiveLoop();
}

void P2PDaemon::startReceiveLoop(){
	asyncReceive(std::bind(&P2PDaemon::handleReceive, this, std::placeholders::_1, std::placeholders::_2));	// Start receive loop.
}

void P2PDaemon::markMessage(api::APIMessage& message) {
	message.set_message_token(next_msg_token++);
}

api::APIMessage P2PDaemon::clientExchange(api::APIMessage request_message) {
	// First, we assign a token to message.
	markMessage(request_message);
	try {
		// Then we send message
		asyncSend(request_message, std::bind(&P2PDaemon::handleSend, this, request_message.message_token(), std::placeholders::_1));
		// Then we receive it. It will be received by receive loop in handleReceive().
		// Then we lock until received.
		auto promise_it = awaiting_receive.find(request_message.message_token());
		auto message_future = promise_it->second.get_future();
		auto reply_message = message_future.get();
		// After receiving we cleanup awaiting_receive.
		awaiting_receive.erase(promise_it);

		// Okay, the message can include something we call remote-throw
		if(reply_message.has_error()){
			throw std::system_error(reply_message.error().error_code(), P2PErrorCategory);
		}

		// Then we return
		return reply_message;
	} catch (std::system_error& e) {
		// On error we prune faulty message from awaiting_receive.
		auto promise_it = awaiting_receive.find(request_message.message_token());
		if(promise_it != awaiting_receive.end()){
			awaiting_receive.erase(promise_it);
		}
		// Then rethrow
		throw;
	}
}

} /* namespace p2pnet */
