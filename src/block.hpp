#ifndef BLOCK_HPP
#define BLOCK_HPP
#include <cstdint>
#include <array>
#include <vector>
#include <cstddef>
#include <sodium.h>
#include "transaction.hpp"

struct block
{
		uint64_t m_height;
		std::array<std::byte, crypto_generichash_BYTES> m_prevHash;
		std::array<std::byte, crypto_generichash_BYTES> m_actualHash;
		std::vector<transaction> m_transaction;
		std::array<std::byte, crypto_sign_BYTES> m_sign;
};

#endif // BLOCK_HPP
