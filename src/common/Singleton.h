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

#ifndef SINGLETON_H_
#define SINGLETON_H_

namespace p2pnet {

template <class T>
class Singleton {
protected:
	Singleton(){};
public:
	static T* getInstance(){
		static T* instance;
		if(!instance){
			instance = new T();
		}
		return instance;
	}
	static void clear(){
		T* instance = getInstance();
		delete instance;
	}

	virtual ~Singleton(){};
};

} /* namespace p2pnet */
#endif /* SINGLETON_H_ */
