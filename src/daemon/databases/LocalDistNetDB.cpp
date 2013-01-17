/*
 * LocalDistNetDB.cpp
 *
 *  Created on: 17.01.2013
 *      Author: gamepad
 */

#include "LocalDistNetDB.h"

namespace p2pnet {
namespace databases {

LocalDistNetDB::LocalDistNetDB() {}
LocalDistNetDB::~LocalDistNetDB() {}

crypto::key_public_t LocalDistNetDB::queryPublicKeyOf(crypto::hash_t peer_id) {
	return this->storage.getPublicKeyOf(peer_id);
}

void LocalDistNetDB::postPublicKeyOf(crypto::hash_t peer_id,
		crypto::key_public_t key_public) {
	this->storage.setPublicKeyOf(peer_id, key_public);
}

net::PeerRoute LocalDistNetDB::queryRouteTo(crypto::hash_t peer_id) {
	return this->storage.getRouteTo(peer_id);
}

void LocalDistNetDB::postRouteOf(crypto::hash_t peer_id,
		net::PeerRoute& route) {
	this->storage.setRouteOf(peer_id, route);
}

} /* namespace databases */
} /* namespace p2pnet */
