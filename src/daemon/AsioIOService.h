/*
 * AsioIOService.h
 *
 *  Created on: 17.12.2012
 *      Author: gamepad
 */

#ifndef ASIOIOSERVICE_H_
#define ASIOIOSERVICE_H_

#include <boost/asio.hpp>

namespace p2pnet {

class AsioIOService {
public:
	static boost::asio::io_service& getIOService() {
		static boost::asio::io_service single_io_service;
		return single_io_service;
	}
};

} /* namespace p2pnet */
#endif /* ASIOIOSERVICE_H_ */
