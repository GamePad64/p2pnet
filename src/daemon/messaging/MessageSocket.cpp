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

#include "handlers/RelayHandler.h"
#include "handlers/KeyExchangeHandler.h"
#include "handlers/ConnectionHandler.h"
#include "../net/TransportSocket.h"

#include "MessageSocket.h"
#include <iostream>

namespace p2pnet {
namespace messaging {

MessageSocket::MessageSocket() {
	m_pks = databases::PersonalKeyStorage::getInstance();

	// Adding (and allocating!) active handlers.
	this->addHandler(new handlers::RelayHandler(this));
	this->addHandler(new handlers::KeyExchangeHandler(this));
	this->addHandler(new handlers::ConnectionHandler(this));
}

MessageSocket::~MessageSocket() {
	for(handlers::MessageHandler* &i : m_handler_list){
		// Deallocating active handlers.
		delete i;
	}
	// They will be removed by m_handler_list destructor.
}

void MessageSocket::reject(Reason reason) {
	throw(new RejectException(reason, "MessageSocket"));
}

void MessageSocket::receivedMessage(net::TransportSocketCallback callback) {
	try {
		protocol::p2pMessage message;

		if(message.ParseFromString(callback.data)){
			/*
		 	 * This message is not encrypted. So, we need to check its type.
			 * Only some types of messages are permitted. These types are
			 * used only when encryption is not possible.
			 */
			switch(message.payload().message_type()){
			case message.payload().KEY_REQUEST:
				break;
			default:
				reject(ENCRYPTION_NEEDED);
			}
		}else{
			// Okay, this message is encrypted OR corrupted.
			auto privkey = databases::PersonalKeyStorage::getInstance()->getMyPrivateKey();
			auto decrypted_message = privkey.decrypt(callback.data);

			if(message.ParseFromString(decrypted_message)){
				/*
				 * This message is decrypted well, doesn't seem to be corrupted (but CRC checks will be performed)
				 */

			}else{
				/*
				 * Well, this message is corrupted or "stale". Stale means that this message is
				 * encrypted using our old
				 */
			}
		}
	} catch(RejectException *e) {
		std::clog << e->what();
		delete e;
	}
	// TODO: Add this TransportSocketLink to database.

	processReceivedMessage(message);
}

void MessageSocket::sendMessage(peer::TH th, protocol::p2pMessage message) {
	auto db_entry = databases::NetDBStorage::getInstance()->getEntry(th);
	if(db_entry.has_ecdsa_public_key()){
		// We need some logic here to choose an optimal route.
		if(db_entry.endpoints().size() > 0){
			net::TransportSocketEndpoint endpoint;
			endpoint.fromProtobuf(db_entry.endpoints(0));
			net::TransportSocket::getInstance()->asyncSendTo(endpoint, message.SerializeAsString());
		}
	}else{
		/*
		 * We should use kinda DHT to resolve TransportSocketEndpoint to this peer. Unimplemented yet.
		 * Same hell on inactive TransportSocketEndpoints.
		 */
	}
}

void MessageSocket::sentMessage(net::TransportSocketCallback callback) {
}

void MessageSocket::processReceivedMessage(protocol::p2pMessage message) {
		MessageGenerator m_generator;
		if (!m_generator.checkMessageCRC32(message))
			reject(Reason::CRC_MISMATCH);

		auto sessionmap = SessionStorage::getInstance();
		Session::pointer session_ptr = (*sessionmap)[message.header().src_th()];

		handlers::MessageHandler::MessageState message_props;

		do {
			message_props.repeat = false;
			message_props.skip = false;
			for(auto handler : m_handler_list){
				handler->processReceivedMessage(message, message_props, session_ptr);
				if(message_props.skip){
					break;
				}
			}
		} while(message_props.repeat);
}

void MessageSocket::addHandler(handlers::MessageHandler* handler_ptr) {
	m_handler_list.push_back(handler_ptr);
}

void MessageSocket::removeHandler(handlers::MessageHandler* handler_ptr) {
	m_handler_list.remove(handler_ptr);
}

} /* namespace messaging */
} /* namespace p2pnet */
