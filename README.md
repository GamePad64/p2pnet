P2PNet Distributed Services
===========================

What is P2PNet?
---------------
**P2PNet** is an infrastructure for building peer-2-peer services. This infrastructure connects multiple computers into one *mesh network*, which is built on modern secure encryption algorithms and is independent of central authorities.

Features
--------

- Searching for devices in your local network. It is called *Local Peer Discovery* in Bittorrent. It works using UDP multicasts.
    
    IPv4 multicast address: 239.192.152.144:28915.  
    IPv6 multicast address: [ff08::BD02]:28915

- *Distributed Hash Table* is used for NetDB storage in distributed network. We use Kademlia DHT.
- *Onion (garlic) routing*. It is a technology often used to protect client and server privacy. Server and client build heavily encrypted tunnels so client cannot find out server's IP and vice versa. This technology is used in Tor and I2P.
- *Group management*. You can create groups of computers that can see each over, share particular services with each other and send encrypted data to each other and to the whole group. The group is controlled by an administrator. The most similar protocol is *JXTA*, Java p2p protocol.

Technology overview
-------------------

P2PNet consists of two components:

- **p2pnetd**, the core of P2PNet. It implements peer-2-peer network protocol and encryption.

- **libp2pnet**, the library, which is used to connect to **p2pnetd**. It provides C++ API, which can be used to develop your p2p apps.

Building and dependencies
-------------------------
This library is written in *C++11*, so you need a modern compiler to build it. Also you need:

#### [Boost C++ Libraries](http://boost.org)
A great set of various C++ libraries.
#### [Protocol Buffers](http://code.google.com/p/protobuf/)
Protocol buffers is a solution for variable-length data serialization. We use it for P2PNet protocol implementation.
#### [Botan](http://botan.randombit.net/)
Botan is a great cryptographic framework written in C++11.
#### [Google C++ Testing Framework](http://code.google.com/p/googletest/)
#### [Google C++ B-tree](https://code.google.com/p/cpp-btree/)
C++ B-tree is a template library that implements ordered in-memory containers based on a B-tree data structure.

