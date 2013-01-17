/*
 * main.cpp
 *
 *  Created on: 03.12.2012
 *      Author: gamepad
 */

#include "Daemon.h"
#include <botan/botan.h>

using namespace p2pnet;

int main(int argc, char** argv){
	Botan::LibraryInitializer init("thread_safe=true");
	Daemon d;
	d.run();

	return 0;
}
