/*
 * You may redistribute this program and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef P2PNET_H_
#define P2PNET_H_

#ifdef __cplusplus
#ifndef WITHOUT_BOOST
#include <boost/asio.hpp>
#endif
#include <cstdint>
#else
#include <stdint.h>
#include <stddef.h>
#endif

#if BUILDING_LIBP2PNET && defined _MSC_VER
#define LIBP2PNET_DLL_EXPORTED __declspec(dllexport)
#elif BUILDING_LIBP2PNET
#define LIBP2PNET_DLL_EXPORTED __attribute__((__visibility__("default")))

#elif defined _MSC_VER
#define LIBP2PNET_DLL_EXPORTED __declspec(dllimport)
#else
#define LIBP2PNET_DLL_EXPORTED
#endif

#ifdef __cplusplus
namespace p2pnet {
#endif

#ifdef __cplusplus
/**
 * This class is used for connection to p2pnetd.
 *
 * It doesn't need to be created explicitly, P2PSocket's default constructor creates a singleton instance of it.
 * So, for writing applications that use only single default p2pnetd instance (and this is likely)
 * you don't need to bother with this class, just create P2PSocket.
 */
class P2PDaemon;
#else
typedef struct P2PDaemon P2PDaemon;
#endif

// end of class P2PDaemon
// class P2PContext

#ifndef __cplusplus
typedef
#endif
enum ContextType {
	/**
	 * Context, which is treated like stream (with congestion control and so on) with preserving bounds of packets, like SCTP.
	 */
	CONTEXT_SEQPACKET = 0,
	CONTEXT_STREAM = 1,
	CONTEXT_DATAGRAM = 2,
	CONTEXT_ANY = 255
}
#ifndef __cplusplus
ContextType
#endif
;

#ifdef __cplusplus
class P2PSocket;
class LIBP2PNET_DLL_EXPORTED P2PContext {
	friend class P2PSocket;
	class Impl; Impl* impl;
protected:
	P2PContext(ContextType context_type, P2PSocket* parent_socket);
public:
	virtual ~P2PContext();

	ContextType getContextType() const;
	uint32_t getContextID() const;
	P2PSocket* getParentSocket();
};
#else
typedef struct P2PSocket P2PSocket;
typedef struct P2PContext P2PContext;
#endif

#ifdef __cplusplus
extern "C" {
#endif
LIBP2PNET_DLL_EXPORTED ContextType p2p_getContextType(P2PContext* context);
LIBP2PNET_DLL_EXPORTED uint32_t p2p_getContextID(P2PContext* context);
LIBP2PNET_DLL_EXPORTED P2PSocket* p2p_getParentSocket(P2PContext* context);
#ifdef __cplusplus
}
#endif

// end of class P2PContext
// class P2PSocket

#ifdef __cplusplus
class P2PNode;
class LIBP2PNET_DLL_EXPORTED P2PSocket {
	friend class P2PContext;

	class Impl; Impl* impl;
protected:
	P2PSocket(P2PNode* parent_node);
public:
	virtual ~P2PSocket();

	// Context-control methods
	P2PContext* createContext(ContextType type);
	P2PContext* acceptContext(ContextType type);

	void closeContext(P2PContext* context_ptr);
	void closeContext(uint32_t context_number);	// "0" is really a synonym to disconnect().
};
#else
//typedef struct P2PSocket P2PSocket;
#endif

#ifdef __cplusplus
extern "C" {
#endif
LIBP2PNET_DLL_EXPORTED void p2p_disconnect(P2PSocket* socket);

LIBP2PNET_DLL_EXPORTED P2PContext* p2p_createContext(P2PSocket* socket, ContextType type);
LIBP2PNET_DLL_EXPORTED P2PContext* p2p_acceptContext(P2PSocket* socket, ContextType type);

// closeContext defined in P2PContext section
#ifdef __cplusplus
}
#endif

// end of class P2PSocket
// class P2PNode

#ifdef __cplusplus
class LIBP2PNET_DLL_EXPORTED P2PNode {
	friend class P2PSocket;
	friend class P2PContext;

	class Impl; Impl* impl;
	P2PNode(std::shared_ptr<P2PDaemon> parent_shared_daemon);
public:
	P2PNode();
	P2PNode(P2PDaemon* parent_daemon);
	virtual ~P2PNode();

	// Connection-control methods
	P2PSocket* accept();
	P2PSocket* connect(std::string SH);

	void bind(std::string base58_private_key);
	void listen(uint32_t max_conn);
};
#else
//typedef struct P2PNode P2PNode;
#endif

#ifdef __cplusplus
extern "C" {
#endif
LIBP2PNET_DLL_EXPORTED P2PNode* p2p_createNode();
LIBP2PNET_DLL_EXPORTED P2PNode* p2p_createNodeOnDaemon(P2PDaemon* parent_socket_manager);
LIBP2PNET_DLL_EXPORTED void p2p_destroyNode(P2PNode* node);

LIBP2PNET_DLL_EXPORTED P2PSocket* p2p_accept(P2PNode* listening_node);
LIBP2PNET_DLL_EXPORTED P2PSocket* p2p_connect(P2PNode* connecting_node, char* SH, size_t SH_length);

LIBP2PNET_DLL_EXPORTED void p2p_bindNode(P2PNode* node, char* base58_private_key);
LIBP2PNET_DLL_EXPORTED void p2p_listenNode(P2PNode* node, uint32_t max_conn);
#ifdef __cplusplus
}
#endif

// end of class P2PNode
// free functions

#ifdef __cplusplus
/**
 * Generates key pair, which could be used in p2pnet functions. Basically, they are ECDSA keys.
 * These keys are presented as strings in Base58 encoding.
 *
 * @param private_key_b58 reference to std::string, where private key will be placed.
 * @param public_key_b58 reference to std::string, where public key will be placed.
 */
LIBP2PNET_DLL_EXPORTED void generateKeyPair(std::string& private_key_b58, std::string& public_key_b58);
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Key pair type. Holds pointers to null-terminated strings of private and public keys.
 */
typedef struct {
	char* private_key;
	char* public_key;
} p2p_KeyPair;
/**
 * Generates key pair, which could be used in p2pnet functions. Basically, they are ECDSA keys.
 * These keys are presented as strings in Base58 encoding.
 *
 * @return structure, that holds key pair.
 */
LIBP2PNET_DLL_EXPORTED p2p_KeyPair p2p_generateKeyPair();
#ifdef __cplusplus
}
#endif

// end of free functions

#ifdef __cplusplus
} /* namespace p2pnet */
#endif

#endif /* P2PNET_H_ */
