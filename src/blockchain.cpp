#include "blockchain.hpp"

block blockchain::get_last_block() {
	if (!m_blocks.empty()) return m_blocks.back();
	else throw std::runtime_error("blockchain is empty!");
}

std::array<std::byte, crypto_sign_PUBLICKEYBYTES> blockchain::get_pk_miner(std::array<std::byte, crypto_sign_PUBLICKEYBYTES> &pk) {
	return pk;
}


void blockchain::add_block(block &bl, std::array<std::byte, crypto_sign_PUBLICKEYBYTES> &pk, utxo &ux) {
	if (crypto_sign_verify_detached(reinterpret_cast<unsigned char*>(bl.m_sign.data()), reinterpret_cast<unsigned char*>(bl.m_actualHash.data()), bl.m_actualHash.size(), reinterpret_cast<unsigned char*>(get_pk_miner(pk).data())) != 0) {
		throw std::invalid_argument("signature is bad");
	} else {
		m_blocks.push_back(bl);
		ux.update(bl);
	}
}

bool blockchain::is_empty() {
	if (m_blocks.empty()) return false;
	else return true;
}
