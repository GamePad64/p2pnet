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
#ifndef EXPORT_H_
#define EXPORT_H_

#if BUILDING_LIBP2PNET && HAVE_VISIBILITY
#define LIBP2PNET_DLL_EXPORTED __attribute__((__visibility__("default")))
#elif BUILDING_LIBP2PNET && defined _MSC_VER
#define LIBP2PNET_DLL_EXPORTED __declspec(dllexport)
#elif defined _MSC_VER
#define LIBP2PNET_DLL_EXPORTED __declspec(dllimport)
#else
#define LIBP2PNET_DLL_EXPORTED
#endif

#endif /* EXPORT_H_ */
