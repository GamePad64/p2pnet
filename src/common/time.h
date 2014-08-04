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
#ifndef TIME_H_
#define TIME_H_

#include <boost/date_time.hpp>

inline std::chrono::system_clock::time_point fromISOTime(const std::string& iso_time){
	return boost::date_time::parse_iso_time<std::chrono::system_clock::time_point>(iso_time, 'T');
}

inline std::string toISOTime(std::chrono::system_clock::time_point point){
	return boost::posix_time::to_iso_string();
}

#endif /* TIME_H_ */
