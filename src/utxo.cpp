#include "utxo.hpp"

uint32_t utxo::get_amount(const std::array<std::byte, crypto_generichash_BYTES> &address){
	if (m_address_to_amount.count(address)==1) return m_address_to_amount.at(address);
	else return 0;
}

void utxo::update(const block &block){
	for (auto &tx: block.m_transaction) {
		for (auto &tx_vin: tx.m_vin){
			auto iterator = m_address_to_amount.find(tx_vin.first);
			(*iterator).second-=tx_vin.second;
			if((*iterator).second==0) m_address_to_amount.erase(iterator);
		}
		for (auto &tx_vout: tx.m_vout) {
			auto iterator = m_address_to_amount.find(tx_vout.first);
			if(iterator==m_address_to_amount.end()) {
				m_address_to_amount.emplace(tx_vout.first, tx_vout.second);
			} else (*iterator).second+=tx_vout.second;
		}
	}
}
