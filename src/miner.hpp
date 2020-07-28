#ifndef MINER_HPP
#define MINER_HPP
#include "mempool.hpp"
#include "block.hpp"
#include "utxo.hpp"

class miner {
	private:
		std::array<std::byte, crypto_sign_PUBLICKEYBYTES> m_pk;
		std::array<std::byte, crypto_sign_SECRETKEYBYTES> m_sk;
		std::vector<transaction> m_txs;
		utxo &ux;
		void generate_block_id(block &bl);
		bool is_transaction_valid(transaction tx);

	public:
		void get_tx(transaction &transactions);
		block mine_block();
		block mine_genesisblock();
		block mine_block(block &prev_block);
		std::array<std::byte, crypto_sign_PUBLICKEYBYTES> get_pk();
		miner(utxo &ux);
};

#endif // MINER_HPP
