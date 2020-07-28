#include "miner.hpp"
#include <cstddef>
#include "blockchain.hpp"
#include <algorithm>

#include <iostream>

void miner::get_tx(transaction &transactions) {
	m_txs.push_back(transactions);
}

void miner::generate_block_id(block & bl) {
	std::vector<std::array<std::byte, crypto_generichash_BYTES>> params_block;
	for (auto &tx: bl.m_transaction) {
		params_block.push_back(tx.m_txid);
	}
	std::sort(params_block.begin(), params_block.end());
	params_block.push_back(bl.m_prevHash);
	std::vector<std::byte> params_block_in_bytes;
	for(auto &el_params_block: params_block) {
		for(auto & el: el_params_block) {
			params_block_in_bytes.push_back(el);
		}
	}
	crypto_generichash(reinterpret_cast<unsigned char*>(bl.m_actualHash.data()), bl.m_actualHash.size(), reinterpret_cast<const unsigned char*>(params_block_in_bytes.data()), params_block_in_bytes.size(), nullptr, 0);
}

bool miner::is_transaction_valid(transaction tx){
	for (const auto &vin: tx.m_vin){
		if (vin.second>ux.get_amount(vin.first)) return false;
	}
	return true;
}

block miner::mine_genesisblock() {
	block bl_gen;
	m_txs.erase(std::remove_if(m_txs.begin(), m_txs.end(),[this](transaction tx){return !is_transaction_valid(tx);}), m_txs.end());
	auto it = std::next(m_txs.begin(), std::min(m_txs.size(), blockchainparams::m_max_tx));
	std::move(m_txs.begin(), it, std::back_inserter(bl_gen.m_transaction));
	m_txs.erase(std::remove_if(m_txs.begin(), m_txs.end(),[this](const transaction &tx ){
		if (std::find(m_txs.begin(), m_txs.end(), tx)==m_txs.end()) return false;
		else return true;
			}),
				m_txs.end());
	bl_gen.m_height=0;
	bl_gen.m_prevHash.fill(static_cast<std::byte>(0));
	generate_block_id(bl_gen);
	crypto_sign_detached(reinterpret_cast<unsigned char*>(bl_gen.m_sign.data()), nullptr, reinterpret_cast<const unsigned char*>(bl_gen.m_actualHash.data()), bl_gen.m_actualHash.size(), reinterpret_cast<const unsigned char*>(m_sk.data()));
	return bl_gen;
}

block miner::mine_block(block &prev_block) {
	block actual_block;
	m_txs.erase(std::remove_if(m_txs.begin(), m_txs.end(),[this](transaction &tx){return !is_transaction_valid(tx);}), m_txs.end());
	auto it = std::next(m_txs.begin(), std::min(m_txs.size(), blockchainparams::m_max_tx));
	std::move(m_txs.begin(), it, std::back_inserter(actual_block.m_transaction));
	m_txs.erase(std::remove_if(m_txs.begin(), m_txs.end(),[this](const transaction &tx ){
		if (std::find(m_txs.begin(), m_txs.end(), tx)==m_txs.end()) return false;
		else return true;
			}),
				m_txs.end());
	actual_block.m_height=prev_block.m_height+1;
	std::copy(std::begin(prev_block.m_actualHash), std::end(prev_block.m_actualHash), std::begin(actual_block.m_prevHash));
	generate_block_id(actual_block);
	crypto_sign_detached(reinterpret_cast<unsigned char*>(actual_block.m_sign.data()), nullptr, reinterpret_cast<const unsigned char*>(actual_block.m_actualHash.data()), actual_block.m_actualHash.size(), reinterpret_cast<const unsigned char*>(m_sk.data()));
	return actual_block;
}

std::array<std::byte, crypto_sign_PUBLICKEYBYTES> miner::get_pk() {
	return m_pk;
}

miner::miner(utxo &ux):
	ux(ux)
{
	crypto_sign_keypair(reinterpret_cast<unsigned char*>(m_pk.data()), reinterpret_cast<unsigned char*>(m_sk.data()));
}
