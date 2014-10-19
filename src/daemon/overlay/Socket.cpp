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
#include "Socket.h"
#include "Connection.h"
#include "OverlayProtocol.pb.h"
#include "PayloadParams.h"
#include "Processor.h"
#include "../../p2pnet.h"

namespace p2pnet {
namespace overlay {

Socket::Socket() : key_provider(this) {
	maximum_connections = getValue<unsigned int>("overlay.connection.limit");
}
Socket::~Socket() {}

void Socket::process(int error, std::shared_ptr<transport::Connection> origin, std::string data) {
	protocol::OverlayMessage message;

	/*
	 * Parsing message into components
	 */
	if(!message.ParseFromString(data)){return;}

	TH src_th(TH::fromBinaryString(message.header().src_th()));
	TH dest_th = message.header().has_dest_th() ? TH::fromBinaryString(message.header().src_th()) : TH();

	log() << "<- OverlayMessage: TH:" << src_th.toBase58() << std::endl;

	/*
	 * Processing message
	 */
	if(dest_th == getKeyProvider()->getKeyInfo()->th){	// Processing message by ourselves as regular message.
		processSelf(origin, message.header(), message.body());
	}else if(dest_th){	// Not processing message by ourselves as we just relay this.
		processRelay(origin, message.header(), message.body());
	}else{	// Processing message by ourselves as connection request.
		processRequest(origin, message.header(), message.body());
	}
}

void Socket::processSelf(std::shared_ptr<transport::Connection> origin,
		const OverlayMessage_Header& header,
		const OverlayMessage_Body& body) {
	TH src_th(TH::fromBinaryString(header.src_th()));
	auto conn_it = connections.find(src_th);
	std::shared_ptr<Connection> conn_ptr;

	if(conn_it == connections.end()){
		conn_ptr = std::make_shared<Connection>(shared_from_this(), src_th);
		connections.insert(std::make_pair(src_th, conn_ptr));
	}else{
		conn_ptr = conn_it->second;
	}

	conn_it->second->process(origin, header, body);
}

void Socket::processRelay(std::shared_ptr<transport::Connection> origin,
		const OverlayMessage_Header& header,
		const OverlayMessage_Body& body) {
}

void Socket::processRequest(std::shared_ptr<transport::Connection> origin,
		const OverlayMessage_Header& header,
		const OverlayMessage_Body& body) {
	if(!getValue<bool>("policies.outgoing_only")){
		// Connect
	}	// else Drop.
}

void Socket::connect(const TH& dest, ConnectCallback callback) {
	auto it = connections.find(dest);
	auto connection_ptr = (it != connections.end()) ? (*it).second : std::make_shared<Connection>(shared_from_this(), dest);
	connection_ptr->connect(callback);
}

void Socket::send(const OverlayMessage& message, SendCallback callback, std::shared_ptr<Connection> connection){
	// Searching for at least one alive transport::Connection
	bool sent = false;
	for(auto transport_connection : connection->transport_connections){
		if(transport_connection->connected()){
			transport_connection->send(
					message.SerializeAsString(),
					std::bind([](int error, std::shared_ptr<transport::Connection> tconn, std::string message, int total, std::shared_ptr<Connection> oconn, SendCallback ocallback){
						ocallback(error, oconn, message, total);
					}, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, connection, callback)
			);
			sent = true;
		}
	}

	if(sent == false){
		connection->setState(Connection::State::SEARCHING_TRANSPORT);
		callback((int)P2PError::no_transport, connection, "", 0);
	}
}

void Socket::registerProcessor(PayloadType payload_type,
		std::shared_ptr<Processor> processor) {
	processors.insert(std::make_pair(payload_type, processor));
}

std::list<std::shared_ptr<Processor>> Socket::getProcessors(PayloadType payload_type){
	auto range = processors.equal_range(payload_type);
	std::list<std::shared_ptr<Processor>> processor_list;

	for(auto it = range.first; it != range.second; it++){
		processor_list.push_back(it->second);
	}

	return processor_list;
}

} /* namespace overlay */
} /* namespace p2pnet */
