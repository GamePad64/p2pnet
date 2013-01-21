/*
 * CryptoHandler.h
 *
 *  Created on: 20.01.2013
 *      Author: gamepad
 */

#ifndef CRYPTOHANDLER_H_
#define CRYPTOHANDLER_H_

#include "../protobuf/protocol.pb.h"
#include "../net/MessageSocket.h"
#include "../../common/crypto/CurrentCipherSet.h"
#include "MessageHandler.h"
#include <forward_list>

namespace p2pnet {
namespace messaging {

class CryptoHandler : public MessageHandler {
	enum SessionState {
		RECEIVED_REQUEST,
		SENT_REQUEST,
		RECEIVED_REPLY,
		SENT_REPLY
	};
	std::map<crypto::hash_t, SessionState> agreement_status;
	crypto::CurrentCipherSet cipherset;
public:
	CryptoHandler();
	virtual ~CryptoHandler();

	void receivedMessage(protocol::p2pMessage p2pmessage_struct, net::packet_info_t message);
	void sentMessage(protocol::p2pMessage p2pmessage_struct, net::packet_info_t message);
};

} /* namespace messaging */
} /* namespace p2pnet */
#endif /* CRYPTOHANDLER_H_ */
