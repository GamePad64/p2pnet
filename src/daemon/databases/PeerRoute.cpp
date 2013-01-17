/*
 * PeerRoute.cpp
 *
 *  Created on: 16.01.2013
 *      Author: gamepad
 */

#include "PeerRoute.h"
#include "../net/asio/UDPSocket.h"
#include "../protobuf/net_dest.pb.h"

namespace p2pnet {
namespace net {

PeerRoute::PeerRoute() {
	// TODO Auto-generated constructor stub

}

PeerRoute::~PeerRoute() {
	// TODO Auto-generated destructor stub
}

std::string PeerRoute::toString() {
	PeerRoute_s peerroute_s;
	for(crypto::hash_t &dest_indir : this->dest_indir){
		peerroute_s.add_dest_indir(std::string(dest_indir.begin(), dest_indir.end()));
	}
	for(net::UDPSocketDestination &dest_udp : this->dest_udp){
		peerroute_s.add_dest_udp(dest_udp.toString());
	}
	return peerroute_s.SerializeAsString();
}

void PeerRoute::fromString(const std::string &from) {
	PeerRoute_s peerroute_s;
	peerroute_s.ParseFromString(from);

	for(const std::string &dest_indir_s : peerroute_s.dest_indir()){
		this->dest_indir.push_back(crypto::hash_t(dest_indir_s.begin(), dest_indir_s.end()));
	}
	for(const std::string &udpsocketdestination_s : peerroute_s.dest_udp()){
		net::UDPSocketDestination dest(udpsocketdestination_s);
		this->dest_udp.push_back(dest);
	}
}

} /* namespace net */
} /* namespace p2pnet */
