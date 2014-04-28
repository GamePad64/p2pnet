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
#ifndef NODE_H_
#define NODE_H_

namespace p2pnet {
namespace p2p {

/*
	std::unique_ptr<Connection> connection;
	SH bound_sh;
	api::APISession* api_session;
public:
	LocalSocket();
	LocalSocket(api::APISession* api_session);
	virtual ~LocalSocket();

	bool isBound();
	SH getLocalSH();

	bool loopback();
	void loopback(bool enabled);
*/

class Node {
	SH bound_sh;
public:
	Node();
	virtual ~Node();

	bool isBound();
	SH getLocalSH();
};

} /* namespace p2p */
} /* namespace p2pnet */

#endif /* NODE_H_ */
