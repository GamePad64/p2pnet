/*
 * NetDBSingleton.h
 *
 *  Created on: 20.01.2013
 *      Author: gamepad
 */

#ifndef NETDBSINGLETON_H_
#define NETDBSINGLETON_H_

#include "LocalDistNetDB.h"
#include "DistNetDB.h"

namespace p2pnet {
namespace databases {

typedef LocalDistNetDB distnetdb_t;
class NetDBSingleton {
protected:
	NetDBSingleton();
public:
	static distnetdb_t* getNetDB(){
		static distnetdb_t* netdb;
		if(!netdb){
			netdb = new distnetdb_t();
		}
		return netdb;
	};
};

} /* namespace databases */
} /* namespace p2pnet */
#endif /* NETDBSINGLETON_H_ */
