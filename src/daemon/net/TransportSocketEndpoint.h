/*
 * TransportSocketEndpoint.h
 *
 *  Created on: 27.01.2013
 *      Author: gamepad
 */

#ifndef TRANSPORTSOCKETENDPOINT_H_
#define TRANSPORTSOCKETENDPOINT_H_

#include <string>

namespace p2pnet {
namespace net {

class TransportSocketEndpoint {
public:
	TransportSocketEndpoint();
	virtual ~TransportSocketEndpoint();

	/*!
	 * Returns Protocol Buffers representation of this TransportSocketEndpoint.
	 * @return binary std::string
	 */
	virtual std::string toString() = 0;
	/*!
	 * This function restores state of TransportSocketEndpoint from Protocol Buffers representation.
	 * @param endpoint_s std::string in binary form
	 */
	virtual void fromString(std::string endpoint_s) = 0;
};

} /* namespace net */
} /* namespace p2pnet */
#endif /* TRANSPORTSOCKETENDPOINT_H_ */
