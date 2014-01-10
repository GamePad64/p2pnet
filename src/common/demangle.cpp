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

// The solution have been copied (with little edit) from: http://stackoverflow.com/questions/281818/unmangling-the-result-of-stdtype-infoname

#include <string>

#ifdef __GNUG__	// For g++ demangling.
#include <cstdlib>
#include <memory>
#include <cxxabi.h>

std::string demangle(const char* name) {
	int status = -4; // Some arbitrary value to eliminate the compiler warning.

	std::unique_ptr<char, void(*)(void*)> res {
		abi::__cxa_demangle(name, NULL, NULL, &status),
				std::free
	};

	return (status==0) ? res.get() : name;
}

#else

// Does nothing if not g++.
std::string demangle(const char* name) {
    return name;
}

#endif
