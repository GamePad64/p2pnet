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
#ifndef OVERLAYSOCKET_H_
#define OVERLAYSOCKET_H_

#include "TH.h"
#include "DHT.h"
#include "KeyProvider.h"

#include "../transport/SocketEndpoint.h"
#include "../../common/Singleton.h"
#include "../../common/Loggable.h"

namespace p2pnet {
namespace overlay {

using namespace protocol;

class Socket : public Loggable, ConfigClient {
public:
	//using SendCallback = std::function<void(int, std::shared_ptr<Connection>, std::string, int)>;	// void(int error_code, SocketEndpoint endpoint, std::string message, int bytes_transferred)
	//using ReceiveCallback = std::function<void(int, std::shared_ptr<Connection>, std::string)>;
	using ConnectCallback = std::function<void(int, std::shared_ptr<Connection>)>;
	//using DisconnectCallback = std::function<void(int, std::shared_ptr<Connection>)>;

	using processor_t = boost::signals2::signal<void(std::shared_ptr<Connection>, const OverlayMessage_Header&, const OverlayMessage_Payload&)>;
private:
	KeyProvider key_provider;
	DHT dht_service;

	std::map<TH, std::shared_ptr<Connection>> connections;
	std::map<std::chrono::system_clock::time_point, decltype(connections)::iterator> timed_destroy_queue;

	std::map<PayloadType, processor_t> processors;

public:
	Socket();
	virtual ~Socket();

	KeyProvider* getKeyProvider(){return &key_provider;}
	DHT* getDHT(){return &dht_service;}

	// Well, you see, it is COMPLETELY DIFFERENT from transport::Socket :)
	std::shared_ptr<Connection> getConnection(const TH& th);	// Nullable. If nullptr, then use connect();
	void registerConnection(std::shared_ptr<Connection> connection);
	void unregisterConnection(std::shared_ptr<Connection> connection);

	void connect(const TH& dest, ConnectCallback);
	void process(int error, std::shared_ptr<Connection> origin, std::string data);	// For invocation with transport::Socket::ReceiveCallback
	void process(std::shared_ptr<Connection> origin, const OverlayMessage_Header& header, const OverlayMessage_Payload& payload);
};

} /* namespace overlay */
} /* namespace p2pnet */

#endif /* OVERLAYSOCKET_H_ */
