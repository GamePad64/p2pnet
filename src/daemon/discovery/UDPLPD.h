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

#ifndef UDPLPD_H_
#define UDPLPD_H_

#include "GenericLPD.h"
#include "../../common/Config.h"
#include "../../common/Loggable.h"
#include "../protobuf/Protocol.pb.h"
#include <boost/asio.hpp>
#include <string>

using namespace boost::asio;

namespace p2pnet {
namespace discovery {

class UDPLPD: public GenericLPD, Loggable {
	void waitBeforeSend();

	/**
	 * This function is invoked on receiving packets. It must perform necessary checks for packet integrity.
	 * Its arguments are from asio::socket, as it works as callback.
	 */
	void processReceived(char* recv_buffer,
			size_t recv_bytes,
			std::shared_ptr<ip::udp::endpoint> mcast_endpoint_ptr);

	std::string getMulticastMessage();

	void send();
	void receive();
protected:
	ConfigManager& m_config;

	/**
	 * This timer is used between sending two messages. Default is to wait 10 seconds.
	 */
	deadline_timer m_timer;
	unsigned int m_timer_seconds = 0;

	//! boost::asio multicast UDP Socket
	ip::udp::socket m_lpd_socket;

	ip::udp::endpoint bind_endpoint;
	ip::udp::endpoint target_endpoint;
public:
	UDPLPD(ConfigManager& config);
	virtual ~UDPLPD();

	// Configuration part.
	virtual void readConfig() = 0;
	void configChanged(){};	// TODO: stub

	// Processing part
	void run();

	void startSendLoop();
	void startReceiveLoop();

};

} /* namespace discovery */
} /* namespace p2pnet */
#endif /* UDPLPD_H_ */
