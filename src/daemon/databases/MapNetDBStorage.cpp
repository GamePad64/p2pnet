/*
 * MapNetDB.cpp
 *
 *  Created on: 13.01.2013
 *      Author: gamepad
 */

#include "MapNetDBStorage.h"

namespace p2pnet {
namespace databases {

MapNetDBStorage::MapNetDBStorage() {}
MapNetDBStorage::~MapNetDBStorage() {}

bool MapNetDBStorage::hasPublicKeyOf(crypto::hash_t peer_id){
	return m_pubkeymap.count(peer_id) != 0;
}
crypto::key_public_t MapNetDBStorage::getPublicKeyOf(crypto::hash_t peer_id){
	return this->m_pubkeymap[peer_id];
}
void MapNetDBStorage::setPublicKeyOf(crypto::hash_t peer_id, crypto::key_public_t key_public){
	this->m_pubkeymap[peer_id] = key_public;
	this->m_hashes.push_front(peer_id);
}

bool MapNetDBStorage::hasRouteTo(crypto::hash_t peer_id){
	return m_routemap.count(peer_id) != 0;
}
net::PeerRoute MapNetDBStorage::getRouteTo(crypto::hash_t peer_id){
	return this->m_routemap[peer_id];
}
void MapNetDBStorage::setRouteOf(crypto::hash_t peer_id, net::PeerRoute& route){
	this->m_routemap[peer_id] = route;
}

} /* namespace databases */
} /* namespace p2pnet */
