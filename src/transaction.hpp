#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP
#include <array>
#include <vector>
#include <cstddef>
#include <sodium.h>
#include <map>

enum class transactiontype {
	normal,
	generate
};

struct transaction {
		transactiontype m_type;
		std::map<std::array<std::byte, crypto_generichash_BYTES>, uint32_t> m_vout;
		std::map<std::array<std::byte, crypto_generichash_BYTES>, uint32_t> m_vin;
		std::map<std::array<std::byte, crypto_sign_PUBLICKEYBYTES>, std::array<std::byte, crypto_sign_BYTES>> m_sign_txid;
		std::array<std::byte, crypto_generichash_BYTES> m_txid;
};

bool operator==(const transaction &lhs, const transaction &rhs);
bool operator!=(const transaction &lhs, const transaction &rhs);

#endif // TRANSACTION_HPP
