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

#include "OverlayProtocol.pb.h"
#include <memory>

namespace p2pnet {
using namespace protocol;
namespace overlay {

class Connection;
class Processor {
	friend class Socket;
	std::weak_ptr<Socket> parent;
protected:
	std::shared_ptr<Socket> getParent(){return parent.lock();};
public:
	Processor(std::weak_ptr<Socket> parent) : parent(parent){}
	virtual ~Processor();

	virtual bool isEncryptionMandatory() const {return true;};	// Encryption is mandatory by default
	virtual void process(std::shared_ptr<Connection> connection, const OverlayMessage_Header& header, const OverlayMessage_Payload& payload) = 0;
};

} /* namespace overlay */
} /* namespace p2pnet */
