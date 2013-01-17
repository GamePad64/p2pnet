/*
 * MapNetDB.h
 *
 *  Created on: 13.01.2013
 *      Author: gamepad
 */

#ifndef MAPNETDBSTORAGE_H_
#define MAPNETDBSTORAGE_H_

#include "PeerRoute.h"
#include <map>
#include <forward_list>

namespace p2pnet {
namespace databases {

class MapNetDBStorage {
private:
	std::forward_list<crypto::hash_t> m_hashes;
	std::map<crypto::hash_t, crypto::key_public_t> m_pubkeymap;
	std::map<crypto::hash_t, net::PeerRoute> m_routemap;

public:
	MapNetDBStorage();
	virtual ~MapNetDBStorage();

	auto getHashIteratorBegin(){return m_hashes.begin();};
	auto getHashIteratorEnd(){return m_hashes.end();};

	//! Public key database
	bool hasPublicKeyOf(crypto::hash_t peer_id);
	crypto::key_public_t getPublicKeyOf(crypto::hash_t peer_id);
	void setPublicKeyOf(crypto::hash_t peer_id, crypto::key_public_t key_public);

	//! Routing database
	bool hasRouteTo(crypto::hash_t peer_id);
	net::PeerRoute getRouteTo(crypto::hash_t peer_id);
	void setRouteOf(crypto::hash_t peer_id, net::PeerRoute& route);
};

} /* namespace databases */
} /* namespace p2pnet */
#endif /* MAPNETDBSTORAGE_H_ */
