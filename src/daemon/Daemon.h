/*
 * Daemon.h
 *
 *  Created on: 01.01.2013
 *      Author: gamepad
 */

#ifndef DAEMON_H_
#define DAEMON_H_

namespace p2pnet {

class Daemon {
public:
	Daemon();
	virtual ~Daemon();

	void run();
};

} /* namespace p2pnet */
#endif /* DAEMON_H_ */
