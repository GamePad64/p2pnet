P2PNet Distributed Overlay Networking Library
=============================================

What is P2PNet?
---------------
P2PNet is computer network layer that allows to transfer messages between multiple computers over the network. It is designed to be modular, so you can communicate not only using network, but using any interface you want.

This library is only called a *library*. In fact it is a *framework*, that consists of two parts: **library** and **daemon**.

**Daemon** starts with system and provides all peer-2-peer communications, cryptography and database management.

**Library** is used to link it with your application and provides API.

They are connected via Unix sockets on Unix-like systems and TCP sockets on Windows systems.

Features
--------
+ Find peers in your local network using UDP multicasts (Local Peer Discovery).
+ Protect data, transferred over the network using strong modern cryptographic algorithms.
+ Create connections to peers, that 

Dependencies
------------
This library is written in *C++11*, so you need a modern compiler to build it. Also you need:

#### [Boost](http://boost.org) 1.52.0
All networking stuff is provided by Boost::Asio.
#### [Protocol Buffers](http://code.google.com/p/protobuf/)  2.5.0
For protocol implementation. All data serialization is implemented using Protocol Buffers.
#### [Botan](http://botan.randombit.net/) 1.11.0
#### [Google C++ Testing Framework](http://code.google.com/p/googletest/) 1.6.0

Each library is in development version with stable API.

# IT is not ready yet 

* IP address for IPv4: 239.192.152.144:28915
* IP address for IPv6: [ff08::BD02]:28915

#### Maybe, in future (In a galaxy far, far away...)
+ Bonjour Discovery
Only for Macs, only by special demand. Also, I don't have a Mac ;).
Bonjour is a Zeroconf system for discovering computers in the local network.
+ Avahi Discovery
Probably, only for Linux, not sure.
Avahi is free&open implementation of Zeroconf.
+ DBus/Binder RPC interface.
DBus is a classic RPC system on Linux, Binder is sort of modern, forked from Android.