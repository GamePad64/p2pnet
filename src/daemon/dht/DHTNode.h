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
#ifndef DHTNODE_H_
#define DHTNODE_H_

#include <boost/date_time.hpp>

using boost::posix_time::seconds;

namespace p2pnet {
namespace dht {

class DHTNode {
protected:
	DHTNode();
public:
	enum class Reliability {
		/**
		 * A good node is a node has responded to one of our queries within the last 15 minutes.
		 * A node is also good if it has ever responded to one of our queries and has sent us a query within the last 15 minutes.
		 */
		GOOD,
		/**
		 * After 15 minutes of inactivity, a node becomes questionable.
		 */
		QUESTIONABLE,
		/**
		 * Nodes become bad when they fail to respond to multiple queries in a row.
		 */
		BAD
	};	/// From BEP-0005 ( http://www.bittorrent.org/beps/bep_0005.html )

	virtual ~DHTNode(){};
	virtual crypto::Hash getHash() const;

	virtual std::string getSerializedContact() const;
	virtual void putSerializedContact(const std::string& serialized_contact);
	virtual seconds getUptime() const;

	virtual Reliability getReliability() const;

	virtual bool isExchangeable() const;
};

} /* namespace dht */
} /* namespace p2pnet */

#endif /* DHTNODE_H_ */
