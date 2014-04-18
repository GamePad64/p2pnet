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

#include <memory>

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

template <class T>
class SharedSingleton {
protected:
	SharedSingleton(){};
public:
	static std::shared_ptr<T> getInstance(){
		static std::weak_ptr<T> instance;
		std::shared_ptr<T> returned_ptr = instance.lock();
		if(!returned_ptr){
			returned_ptr = std::make_shared<T>();
			instance = returned_ptr;
		}
		return returned_ptr;
	}

	virtual ~SharedSingleton(){};
};

} /* namespace p2pnet */
#endif /* SINGLETON_H_ */
