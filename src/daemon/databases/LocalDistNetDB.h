/*
 * LocalDistNetDB.h
 *
 *  Created on: 17.01.2013
 *      Author: gamepad
 */

#ifndef LOCALDISTNETDB_H_
#define LOCALDISTNETDB_H_

#include "DistNetDB.h"

namespace p2pnet {
namespace databases {

class LocalDistNetDB: public p2pnet::databases::DistNetDB {
public:
	LocalDistNetDB();
	virtual ~LocalDistNetDB();

	virtual crypto::key_public_t queryPublicKeyOf(crypto::hash_t peer_id);
	virtual void postPublicKeyOf(crypto::hash_t peer_id, crypto::key_public_t key_public);

	virtual net::PeerRoute queryRouteTo(crypto::hash_t peer_id);
	virtual void postRouteOf(crypto::hash_t peer_id, net::PeerRoute& route);
};

} /* namespace databases */
} /* namespace p2pnet */
#endif /* LOCALDISTNETDB_H_ */
