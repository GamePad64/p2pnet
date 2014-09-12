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

#include <string>
#include <memory>
#include "InterfaceEndpoint.h"

namespace p2pnet {
namespace transport {

class Socket;
class SocketEndpoint final {
	friend class Socket;
protected:
	std::shared_ptr<InterfaceEndpoint> interface_endpoint;
	Socket* parent;

	/**
	 * Creates new endpoint instance using specified interface ID.
	 * @param id
	 */
	void resetEndpoint(std::string name);
public:
	SocketEndpoint(Socket* parent);
	SocketEndpoint(const SocketEndpoint& tse);

	SocketEndpoint(Socket* parent, const std::shared_ptr<InterfaceEndpoint> interface_endpoint);
	~SocketEndpoint(){};

	// Operators
	SocketEndpoint& operator =(const SocketEndpoint& tse);
	bool operator ==(const SocketEndpoint& tse) const;
	bool operator <(const SocketEndpoint& tse) const;

	explicit operator bool();

	// Endpoint information getters
	std::string getPrefix() const;
	inline std::shared_ptr<InterfaceEndpoint> getInterfaceEndpoint(){
		return interface_endpoint;
	}

	bool isTransferable() const;

	/*
	 * Human readable strings operation.
	 */
	const char delimiter = ':';

	void fromString(std::string string);
	std::string toString() const;
};

} /* namespace net */
} /* namespace p2pnet */
