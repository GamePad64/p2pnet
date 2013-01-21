/*
 * CryptoHandler.cpp
 *
 *  Created on: 20.01.2013
 *      Author: gamepad
 */

#include "CryptoHandler.h"
#include "../databases/NetDBSingleton.h"
#include <algorithm>

namespace p2pnet {
namespace messaging {

CryptoHandler::CryptoHandler() {}
CryptoHandler::~CryptoHandler() {}

void CryptoHandler::receivedMessage(protocol::p2pMessage p2pmessage_struct, net::packet_info_t message) {
	crypto::hash_t src_id(p2pmessage_struct.message_header().src_id().begin(), p2pmessage_struct.message_header().src_id().end());
	switch(p2pmessage_struct.message_type()){

	case p2pmessage_struct.AGREEMENT:	{
		protocol::Agreement_msg agreement_msg_struct;
		agreement_msg_struct.ParseFromString(p2pmessage_struct.message_s());

		crypto::key_public_t src_pubkey(agreement_msg_struct.src_pubkey().begin(), agreement_msg_struct.src_pubkey().end());

		if(cipherset.checkHash(agreement_msg_struct.src_pubkey(), src_id)){
			auto agreement_pair_iterator = agreement_status.find(src_id);
			if(agreement_pair_iterator == agreement_status.end()){
				agreement_status[src_id] = RECEIVED_REQUEST;
				// Then send reply
			}
		}
	}
	break;

	default:
		break;
	}
}

void CryptoHandler::sentMessage(protocol::p2pMessage p2pmessage_struct, net::packet_info_t message) {
	switch(p2pmessage_struct.message_type()){
	case p2pmessage_struct.AGREEMENT:	{
		std::string dest_id_string = p2pmessage_struct.message_header().dest_id();
		crypto::hash_t dest_id(dest_id_string.begin(), dest_id_string.end());
	}
	break;

	default:
		break;
	}
}

} /* namespace messaging */
} /* namespace p2pnet */
