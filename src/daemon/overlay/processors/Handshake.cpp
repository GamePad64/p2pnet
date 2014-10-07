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
#include "Handshake.h"

namespace p2pnet {
namespace overlay {
namespace processors {

Handshake::Handshake() {}

Handshake::~Handshake() {}

void Handshake::process(std::shared_ptr<Connection> connection, const protocol::OverlayMessage_Header& header, const protocol::OverlayMessage_Payload& payload) {

}

void Connection::performRemoteKeyRotation(std::pair<crypto::PrivateKeyDSA, TH> previous_keys){
	if(state == State::ESTABLISHED){
		OverlayMessage message = genMessageSkel(previous_keys.second, remoteTH(), Priority::RELIABLE);
		Handshake handshake;

		handshake.set_stage(handshake.PUBKEY_ROTATION);
		handshake.mutable_pubkey()->CopyFrom(genPubkeyStage(handshake.PUBKEY_ROTATION, previous_keys.first));

		sendMessage(message);
	}
}


Handshake_PubkeyStage Connection::genPubkeyStage(Handshake::Stage for_stage, boost::optional<const crypto::PrivateKeyDSA&> our_hist_key){
	Handshake_PubkeyStage stage;

	std::string new_key_s = localPublicKey().toBinaryString();
	stage.mutable_signed_content()->set_new_ecdsa_key(new_key_s);

	stage.mutable_signed_content()->set_expiration_time(system_clock::to_time_t(getLocalKeyInfo().expiration_time));
	stage.mutable_signed_content()->set_lose_time(system_clock::to_time_t(getLocalKeyInfo().lose_time));

	if(for_stage == Handshake::PUBKEY_ROTATION){
		std::string old_key_s = our_hist_key->derivePublicKey().toBinaryString();
		stage.mutable_signed_content()->set_old_ecdsa_key(old_key_s);

		auto signed_content = stage.signed_content().SerializeAsString();

		stage.set_old_signature(our_hist_key->sign(signed_content));
		stage.set_new_signature(localPrivateKey().sign(signed_content));
	}

	return stage;
}

Handshake_ECDHStage Connection::genECDHStage(){
	Handshake_ECDHStage stage;

	if(!session_ecdh_key.isPresent())
		session_ecdh_key = crypto::ECDH::generateNewKey();

	auto public_ecdh_component = session_ecdh_key.derivePublicKey();

	stage.set_src_ecdh_pubkey(public_ecdh_component);
	stage.set_signature(localPrivateKey().sign(public_ecdh_component));

	return stage;
}

void Connection::processHandshake(const OverlayMessage_Header& header, std::string serialized_payload) {
	Handshake handshake;
	handshake.ParseFromString(serialized_payload);

	switch(handshake.stage()){
		case Handshake_Stage_PUBKEY:
		case Handshake_Stage_PUBKEY_ACK:
		case Handshake_Stage_PUBKEY_ROTATION:
			processPubkeyStage(header, handshake);
			break;

		case Handshake_Stage_ECDH:
		case Handshake_Stage_ECDH_ACK:
			processECDHStage(header, handshake);
			break;
	}
}

void Connection::processPubkeyStage(const OverlayMessage_Header& header, Handshake handshake_payload){
	auto new_ecdsa_key = crypto::PublicKeyDSA::fromBinaryString(handshake_payload.pubkey().signed_content().new_ecdsa_key());

	if(crypto::Hash(new_ecdsa_key) == remoteTH()){
		node_ptr->setPublicKey(new_ecdsa_key);
	}else if(handshake_payload.stage() == handshake_payload.PUBKEY_ROTATION &&
			handshake_payload.pubkey().signed_content().has_old_ecdsa_key()){
		auto signed_content = handshake_payload.pubkey().signed_content().SerializeAsString();
		auto old_ecdsa_key = crypto::PublicKeyDSA::fromBinaryString(handshake_payload.pubkey().signed_content().old_ecdsa_key());

		if(old_ecdsa_key.verify(signed_content, handshake_payload.pubkey().old_signature()) &&
				new_ecdsa_key.validate() &&
				new_ecdsa_key.verify(signed_content, handshake_payload.pubkey().new_signature())){
			// So, the new key is good
			node_ptr->setPublicKey(new_ecdsa_key);
		}
	}else{
		return;
	}

	if(handshake_payload.stage() != handshake_payload.PUBKEY_ROTATION){
		auto reply = genMessageSkel(header);
		Handshake new_handshake_payload;

		switch(handshake_payload.stage()){
			case Handshake_Stage_PUBKEY:
				new_handshake_payload.mutable_pubkey()->CopyFrom(genPubkeyStage(Handshake_Stage_PUBKEY_ACK));
				break;
			case Handshake_Stage_PUBKEY_ACK:
				new_handshake_payload.mutable_pubkey()->CopyFrom(genECDHStage());
				break;
			default:
				;
		}
		addPayload(new_handshake_payload.SerializeAsString(), PayloadType::HANDSHAKE, reply);
		sendMessage(reply);
	}
}

void Connection::processECDHStage(const OverlayMessage_Header& header, Handshake handshake_payload){
	if(!session_ecdh_key.isPresent())
		session_ecdh_key = crypto::ECDH::generateNewKey();

	auto salt_v = (remoteTH() ^ localTH()).toBinaryString();
	auto derived_aes_string = session_ecdh_key.deriveSymmetricKey(crypto::AES::vectorSize(),
			handshake_payload.ecdh().src_ecdh_pubkey(), salt_v);
	session_aes_key = crypto::AES::fromBinaryString(derived_aes_string);

	log() << "Received ECDH public key from: TH:" << remoteTH().toBase58() << std::endl;

	auto reply = genMessageSkel(header);
	Handshake new_handshake_payload;

	if(handshake_payload.stage() == handshake_payload.ECDH && state == State::PUBKEY_RECEIVED){
		/* We need to send back ECDH_ACK */
		new_handshake_payload.mutable_ecdh()->CopyFrom(genECDHStage());

		addPayload(new_handshake_payload.SerializeAsString(), PayloadType::HANDSHAKE, reply);
		sendMessage(reply);
	}

	setState(State::ESTABLISHED);
	log() << "AES encrypted connection with TH:" << remoteTH().toBase58() << " established" << std::endl;
}

} /* namespace processors */
} /* namespace overlay */
} /* namespace p2pnet */
