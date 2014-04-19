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
	class Impl; Impl* impl;
public:
	P2PContext(ContextType context_type, P2PSocket* parent_socket);
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
// class P2PContext

#ifdef __cplusplus

class LIBP2PNET_DLL_EXPORTED P2PSocket {
	class Impl; Impl* impl;
	P2PSocket(std::shared_ptr<P2PDaemon> parent_shared_daemon);
public:
	P2PSocket();
	P2PSocket(P2PDaemon* parent_daemon);
	P2PSocket(std::string base58_private_key);
	P2PSocket(std::string base58_private_key, P2PDaemon* parent_daemon);

	virtual ~P2PSocket();

	// Connection-control methods
	P2PSocket* accept();
	void connect(std::string SH);
	void disconnect();

	void bind(std::string base58_private_key);	// Assigns private key to this socket.
	void listen();

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
LIBP2PNET_DLL_EXPORTED P2PSocket* p2p_createSocket();
LIBP2PNET_DLL_EXPORTED P2PSocket* p2p_createSocketOnDaemon(P2PDaemon* parent_socket_manager);
LIBP2PNET_DLL_EXPORTED void p2p_destroySocket(P2PSocket* socket);

LIBP2PNET_DLL_EXPORTED P2PSocket* p2p_accept(P2PSocket* listening_socket);
LIBP2PNET_DLL_EXPORTED void p2p_connect(P2PSocket* connecting_socket, char* SH, size_t SH_length);
LIBP2PNET_DLL_EXPORTED void p2p_disconnect(P2PSocket* socket);

LIBP2PNET_DLL_EXPORTED void p2p_bindSocket(P2PSocket* socket, char* base58_private_key, size_t base58_private_key_length);
LIBP2PNET_DLL_EXPORTED void p2p_listenSocket(P2PSocket* socket);

LIBP2PNET_DLL_EXPORTED P2PContext* p2p_createContext(P2PSocket* socket, ContextType type);
LIBP2PNET_DLL_EXPORTED P2PContext* p2p_acceptContext(P2PSocket* socket, ContextType type);

// closeContext defined in P2PContext section
#ifdef __cplusplus
}
#endif

// end of class P2PContext

#ifdef __cplusplus
} /* namespace p2pnet */
#endif

#endif /* P2PNET_H_ */
