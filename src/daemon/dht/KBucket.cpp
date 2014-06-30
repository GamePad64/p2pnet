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
#include "KBucket.h"

namespace p2pnet {
namespace dht {

KBucket::KBucket(const crypto::Hash& node_hash, uint16_t k) : index(0), splittable(true), k(k) {
	node_hash_ptr = std::make_shared<const crypto::Hash>(node_hash);
}

KBucket::KBucket(int index, bool splittable, std::shared_ptr<const crypto::Hash> node_hash_ptr, uint16_t k) :
		index(index),
		splittable(splittable),
		node_hash_ptr(node_hash_ptr) {}

KBucket::~KBucket() {}

void KBucket::split() {
	if(isSplit() || !splittable)
		return;

	auto my_hash_bit = getBit(*node_hash_ptr, index+1);

	split_buckets.first = std::unique_ptr<KBucket>(new KBucket(index+1, !my_hash_bit, node_hash_ptr, k));
	split_buckets.second = std::unique_ptr<KBucket>(new KBucket(index+1, my_hash_bit, node_hash_ptr, k));

	for(auto node : bucket_contents){
		determineBucket(node->getHash())->bucket_contents.push_back(node);
	}

	bucket_contents.clear();
}

KBucket* KBucket::determineBucket(const crypto::Hash& hash) {
	if(!isSplit())
		return this;

	if( getBit(hash, index+1) == false )
		return split_buckets.first.get();
	else
		return split_buckets.second.get();
}

void KBucket::cleanup(){
	for(auto it = bucket_contents.begin(); it != bucket_contents.end(); it++){
		if((*it)->getReliability() == DHTNode::Reliability::BAD){
			bucket_contents.erase(it);
		}
	}
}

bool KBucket::getBit(const crypto::Hash& hash, int bit_index){
	int split_byte = bit_index / 8;
	int split_bit = bit_index % 8;

	return (hash.toBinaryVector()[split_byte] >> (7-split_bit)) & 0x1;
}

bool KBucket::isSplit() const {
	return split_buckets.first != nullptr && split_buckets.second != nullptr;
}

void KBucket::addNode(DHTNode* node, bool force) {
	auto node_hash = node->getHash();

	if(!isSplit()){
		if(bucket_contents.size() < k){	// Then we don't need this cleanup routine
			bucket_contents.push_back(node);
		}else{
			// Okay, we need to cleanup. So, we leave only GOOD nodes.
			std::remove_if(bucket_contents.begin(), bucket_contents.end(),
					[](DHTNode* pred_node){return pred_node->getReliability() == DHTNode::Reliability::BAD;}
			);

			if(bucket_contents.size() < k){
				bucket_contents.push_back(node);
			}else if(splittable){
				split();
				determineBucket(node_hash)->addNode(node);
			}
		}
	}else{
		determineBucket(node_hash)->addNode(node);
	}
}

void KBucket::removeNode(DHTNode* node) {
	auto node_hash = node->getHash();

	if(!isSplit()){
		auto erase_it = std::find(bucket_contents.begin(), bucket_contents.end(), node);
		if(erase_it != bucket_contents.end())
			bucket_contents.erase(erase_it);
	}else{
		determineBucket(node_hash)->removeNode(node);
	}
}

std::list<DHTNode*> KBucket::getClosest(const crypto::Hash& hash, int n) {
	std::list<DHTNode*> result_list;

	if(!isSplit()){
		decltype(n) i = 0;
		auto it = bucket_contents.begin();
		while(i < n || it != bucket_contents.end()){
			result_list.push_back(*it);

			i++;
		}
		return result_list;
	}else{
		auto first = determineBucket(hash);
		auto second = (split_buckets.first == first ? split_buckets.second : split_buckets.first);

		result_list = first->getClosest(hash, n);

		if(result_list.size() < n){
			std::list<DHTNode*> result_list_appendix = second->getClosest(hash, n);
			result_list.insert(result_list.end(), result_list_appendix.begin(), result_list_appendix.end());
		}

		return result_list;
	}
}

int KBucket::count() const {
	if(isSplit()){
		return split_buckets.first->count() + split_buckets.second->count();
	}else{
		return bucket_contents.size();
	}
}

} /* namespace dht */
} /* namespace p2pnet */
