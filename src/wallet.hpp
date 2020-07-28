#ifndef WALLET_HPP
#define WALLET_HPP
#include <array>
#include <cstddef>
#include <sodium.h>
#include "transaction.hpp"

class wallet {
	public:
		std::array<std::byte, crypto_generichash_BYTES>	get_address();
		transaction send_to_address(std::array<std::byte, crypto_generichash_BYTES> &address, uint32_t amount);
		transaction generate_tx(uint32_t amount);

		wallet();

	private:
		std::array<std::byte, crypto_sign_PUBLICKEYBYTES> m_pk;
		std::array<std::byte, crypto_sign_SECRETKEYBYTES> m_sk;
		std::array<std::byte, crypto_generichash_BYTES> m_wallet_address;
		std::array<std::byte, crypto_generichash_BYTES>	get_new_address();
};

#endif // WALLET_HPP
