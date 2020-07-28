#ifndef BLOCKCHAIN_HPP
#define BLOCKCHAIN_HPP
#include <cstddef>
#include "block.hpp"
#include "miner.hpp"

class blockchain {
	public:
		block get_last_block();
		void add_block(block &bl, std::array<std::byte, crypto_sign_PUBLICKEYBYTES> &pk, utxo &ux);
		std::array<std::byte, crypto_sign_PUBLICKEYBYTES> get_pk_miner(std::array<std::byte, crypto_sign_PUBLICKEYBYTES> &pk);
		bool is_empty();

	private:
		std::vector<block> m_blocks;

};

struct blockchainparams {
	static constexpr std::size_t m_max_tx = 10;

};

#endif // BLOCKCHAIN_HPP
