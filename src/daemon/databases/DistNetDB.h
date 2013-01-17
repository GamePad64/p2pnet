/*
 * DistNetDB.h
 *
 *  Created on: 14.01.2013
 *      Author: gamepad
 */

#ifndef DISTNETDB_H_
#define DISTNETDB_H_

#include "PeerRoute.h"
#include "MapNetDBStorage.h"

namespace p2pnet {
namespace databases {

class DistNetDB {
protected:
	typedef MapNetDBStorage storage_t;
	storage_t storage;
public:
	DistNetDB();
	virtual ~DistNetDB();

	virtual crypto::key_public_t queryPublicKeyOf(crypto::hash_t peer_id) = 0;
	virtual void postPublicKeyOf(crypto::hash_t peer_id, crypto::key_public_t key_public) = 0;

	virtual net::PeerRoute queryRouteTo(crypto::hash_t peer_id) = 0;
	virtual void postRouteOf(crypto::hash_t peer_id, net::PeerRoute& route) = 0;

	storage_t& getMappedStorage(){return storage;};
};

} /* namespace databases */
} /* namespace p2pnet */
#endif /* DISTNETDB_H_ */
