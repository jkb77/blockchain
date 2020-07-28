#ifndef UTXO_HPP
#define UTXO_HPP
#include <array>
#include <cstddef>
#include <sodium.h>
#include <vector>
#include <map>
#include "block.hpp"

class utxo {
	std::map<std::array<std::byte, crypto_generichash_BYTES>, uint32_t> m_address_to_amount;

	public:
		uint32_t get_amount(const std::array<std::byte, crypto_generichash_BYTES> &address);
		void update(const block &block);
};

#endif // UTXO_HPP
