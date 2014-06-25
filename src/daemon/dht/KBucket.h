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
#ifndef KBUCKET_H_
#define KBUCKET_H_

namespace p2pnet {
namespace dht {

/**
 * It is basically an implementation of Bittorrent-like Kademlia K-buckets.
 *
 * See more about K-buckets:
 * JS implementation of K-buckets: https://github.com/tristanls/k-bucket
 * BitTorrent BEP-5: http://www.bittorrent.org/beps/bep_0005.html
 */
class KBucket {
	const int index;
	std::pair<std::unique_ptr<KBucket>, std::unique_ptr<KBucket>> split_buckets = {nullptr, nullptr};
	std::list<DHTNode*> bucket_contents;
	const bool splittable;

	std::shared_ptr<const crypto::Hash> node_hash_ptr;

	KBucket(int index, bool splittable, std::shared_ptr<const crypto::Hash> node_hash_ptr);

	/**
	 * Splits this KBucket into two. Uses index to determine, by whick byte shall we split.
	 */
	void split();

	/**
	 * Returns pointer to KBucket, which can store this hash. If KBucket is not split yet, it returns "this"
	 * @param hash
	 * @return
	 */
	KBucket* determineBucket(const crypto::Hash& hash);

	/**
	 * Gets specific bit in hash
	 * @param hash
	 * @param bit_index
	 * @return
	 *
	 * TODO: Watch behavior of this function on machines with different endianess.
	 */
	bool getBit(const crypto::Hash& hash, int bit_index);

	bool isSplit() const;
public:
	/**
	 * KBucket public constructor. It constructs new KBucket, using node_hash as its own node's hash.
	 * @param node_hash Own node's hash.
	 */
	KBucket(const crypto::Hash& node_hash);
	virtual ~KBucket();	// Unique_ptr's will delete nested k-buckets recursively!

	void addNode(DHTNode* node);
	void removeNode(DHTNode* node);
	std::list<DHTNode*> getClosest(const crypto::Hash& hash, int n = 0);

	/**
	 * Counts elements in this KBucket. This function walks around all children KBuckets.
	 * @return Elements in KBucket.
	 */
	int count();
};

} /* namespace dht */
} /* namespace p2pnet */

#endif /* KBUCKET_H_ */
