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
#pragma once

#include "TH.h"
#include "PayloadParams.h"

#include "KeyProvider.h"

#include "Processor.h"
#include "OverlayProtocol.pb.h"

#include "../transport/Connection.h"
#include "../transport/SocketEndpoint.h"
#include "../../common/Loggable.h"

#include <memory>

namespace p2pnet {
using namespace protocol;
namespace overlay {

class Connection;
class Socket : public Loggable, ConfigClient, public std::enable_shared_from_this<Socket> {
public:
	using SendCallback = std::function<void(int, std::shared_ptr<Connection>, std::string, int)>;	// void(int error_code, SocketEndpoint endpoint, std::string message, int bytes_transferred)
	//using ReceiveCallback = std::function<void(int, std::shared_ptr<Connection>, std::string)>;
	using ConnectCallback = std::function<void(int, std::shared_ptr<Connection>)>;
	//using DisconnectCallback = std::function<void(int, std::shared_ptr<Connection>)>;
private:
	unsigned int maximum_connections;

	KeyProvider key_provider;

	std::map<TH, std::shared_ptr<Connection>> connections;
	std::map<std::chrono::system_clock::time_point, decltype(connections)::iterator> timed_destroy_queue;

	std::multimap<PayloadType, std::shared_ptr<Processor>> processors;

	void processSelf(std::shared_ptr<transport::Connection> origin, const OverlayMessage_Header& header, const OverlayMessage_Body& body);
	void processRelay(std::shared_ptr<transport::Connection> origin, const OverlayMessage_Header& header, const OverlayMessage_Body& body);
	void processRequest(std::shared_ptr<transport::Connection> origin, const OverlayMessage_Header& header, const OverlayMessage_Body& body);
public:
	Socket();
	virtual ~Socket();

	KeyProvider* getKeyProvider(){return &key_provider;}

	void onConnect(std::shared_ptr<Connection> connection);
	void onDisconnect(std::shared_ptr<Connection> connection);

	// Well, you see, it is COMPLETELY DIFFERENT from transport::Socket :)
	void registerConnection(std::shared_ptr<Connection> connection);
	void unregisterConnection(std::shared_ptr<Connection> connection);

	void connect(const TH& dest, ConnectCallback callback);
	void process(int error, std::shared_ptr<transport::Connection> origin, std::string data);	// For invocation with transport::Socket::ReceiveCallback
    void send(const OverlayMessage& message, Socket::SendCallback callback, std::shared_ptr<Connection> connection);

	void registerProcessor(PayloadType payload_type, std::shared_ptr<Processor> processor);
	std::list<std::shared_ptr<Processor>> getProcessors(PayloadType payload_type);
};

} /* namespace overlay */
} /* namespace p2pnet */
