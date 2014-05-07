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
#include "P2PSession.h"

#include "P2PDaemon.h"

#include <future>

namespace p2pnet {

P2PSession::P2PSession(P2PDaemon* parent_daemon) {
	this->parent_daemon = parent_daemon;
	m_io_service_work = std::unique_ptr<boost::asio::io_service::work>(new boost::asio::io_service::work(m_io_service));

	socket_thread = new std::thread([&](){m_io_service.run();});
}

P2PSession::~P2PSession() {
	m_io_service_work.reset();
	m_io_service.stop();
	if(socket_thread->joinable())
		socket_thread->join();
	delete socket_thread;
}

void P2PSession::handleSend(std::error_condition& error) {}

void P2PSession::handleReceive(api::APIMessage message, std::error_condition& error) {
	auto token = message.message_token();

	if(message.has_message_token()){
		auto promise_it = awaiting_receive.find(token);
		promise_it->second.set_value(message);
	}

	if(error.value() != boost::asio::error::operation_aborted){	// It is invoked on P2PDaemon destruction.
		// Continue receive loop
		receiveLoop();
	}
}

void P2PSession::receiveLoop(){
	asyncReceive(getReceiveHandler());	// Start receive loop.
	m_io_service_work.reset();
}

uint32_t P2PSession::markMessage(api::APIMessage& message) {
	uint32_t token = next_msg_token++;
	message.set_message_token(token);
	return token;
}

api::SendHandler P2PSession::getSendHandler(){
	return std::bind(&P2PSession::handleSend, this, std::placeholders::_1);
}

api::ReceiveHandler P2PSession::getReceiveHandler(){
	return std::bind(&P2PSession::handleReceive, this, std::placeholders::_1, std::placeholders::_2);
}

boost::asio::io_service& P2PSession::getIOService(){
	return m_io_service;
}

void P2PSession::asyncSend(api::APIMessage data, api::SendHandler handler){
	parent_daemon->asyncSend(data, handler);
}

void P2PSession::asyncReceive(api::ReceiveHandler handler){
	parent_daemon->asyncReceive(handler);
}

api::APIMessage P2PSession::clientExchange(api::APIMessage request_message) {
	// First, we assign a token to message.
	auto token = markMessage(request_message);
	try {
		auto promise_it_pair = awaiting_receive.insert(std::make_pair(token, std::promise<api::APIMessage>()));
		auto reply_future = promise_it_pair.first->second.get_future();

		// Then we send message
		asyncSend(request_message, getSendHandler());

		// Then we receive it. It will be received by receive loop in handleReceive().
		api::APIMessage reply_message = reply_future.get();

		// After receiving we cleanup awaiting_receive.
		awaiting_receive.erase(promise_it_pair.first);

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
