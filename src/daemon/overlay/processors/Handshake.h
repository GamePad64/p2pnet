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

#include "Processor.h"
#include "Handshake.pb.h"

namespace p2pnet {
namespace overlay {
namespace processors {

class Handshake: public Processor {
public:
	Handshake();
	virtual ~Handshake();

	protocol::Handshake genPubkeyStage(Handshake::Stage for_stage, boost::optional<const crypto::PrivateKeyDSA&> our_hist_key = boost::none);
	protocol::Handshake genECDHStage();

	void performRemoteKeyRotation(std::pair<crypto::PrivateKeyDSA, TH> previous_keys);

	void processHandshake(const OverlayMessage_Header& header, std::string serialized_payload);

	void processPubkeyStage(const OverlayMessage_Header& header, Handshake handshake_payload);
	void processECDHStage(const OverlayMessage_Header& header, Handshake handshake_payload);

	virtual bool isEncryptionMandatory() const {return false;};	// Encryption is mandatory by default
	void process(std::shared_ptr<Connection> connection, const OverlayMessage_Header& header, const OverlayMessage_Payload& payload);
};

} /* namespace processors */
} /* namespace overlay */
} /* namespace p2pnet */
