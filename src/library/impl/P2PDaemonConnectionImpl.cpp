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
#include "P2PDaemonConnectionImpl.h"

#ifdef BOOST_ASIO_HAS_LOCAL_SOCKETS
#include "daemon_comm/UnixAPIClient.h"
#endif
#include "daemon_comm/APIClient.h"

namespace p2pnet {
namespace impl {

P2PDaemonConnectionImpl::P2PDaemonConnectionImpl(bool autoconnect) :
		m_external_io_service(false) {
	m_io_service = new boost::asio::io_service();
	socket_thread = std::thread([&](){m_io_service->run();});
	if(autoconnect){
		connect();
	}
}

P2PDaemonConnectionImpl::P2PDaemonConnectionImpl(boost::asio::io_service& io_service, bool autoconnect) :
		m_external_io_service(true) {
	m_io_service = &io_service;
	if(autoconnect){
		connect();
	}
}

P2PDaemonConnectionImpl::~P2PDaemonConnectionImpl() {
	if(!m_external_io_service){
		m_io_service->stop();
		if(socket_thread.joinable())
			socket_thread.join();
		delete m_io_service;
	}
}

int P2PDaemonConnectionImpl::connect() {
	int ec = 0;
#ifdef BOOST_ASIO_HAS_LOCAL_SOCKETS
	try {
		m_api_client = std::make_shared<api::unix::UnixAPIClient>(*m_io_service);
	} catch(...){
		ec = 1;
	}
#endif
	return ec;
}

bool P2PDaemonConnectionImpl::is_connected() {
	return bool(m_api_client);
}

std::shared_ptr<api::APIClient> P2PDaemonConnectionImpl::getAPIClient() {
	return m_api_client;
}

} /* namespace impl */
} /* namespace p2pnet */
