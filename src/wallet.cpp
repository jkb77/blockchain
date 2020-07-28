#include "wallet.hpp"
#include <cstdint>

std::array<std::byte, 4> get_array_byte(uint32_t number_of_uint32){
	std::array<std::byte, 4> array_of_byte;
	for (std::size_t i = 0; i < 4; i++)
			 array_of_byte[3 - i] = static_cast<std::byte>(number_of_uint32 >> (i * 8));
	return array_of_byte;
}

std::array<std::byte, crypto_generichash_BYTES> wallet::get_new_address() {
	std::array<std::byte,crypto_generichash_BYTES> out;
	crypto_generichash(reinterpret_cast<unsigned char*>(out.data()), out.size(), reinterpret_cast<const unsigned char*>(m_pk.data()), m_pk.size(), nullptr, 0);
	return out;
}

std::array<std::byte, crypto_generichash_BYTES> wallet::get_address() {
	return m_wallet_address;
}

transaction wallet::send_to_address(std::array<std::byte, crypto_generichash_BYTES> &address, uint32_t amount){
	transaction tx;
	tx.m_type=transactiontype::normal;
	tx.m_vout.emplace(address, amount);
	tx.m_vin.emplace(m_wallet_address, amount);
	std::vector<std::byte>tx_vin_vout;
	for(auto &tx_vin: tx.m_vin){
		std::copy(std::begin(tx_vin.first), std::end(tx_vin.first), std::back_inserter(tx_vin_vout));
		std::array<std::byte, 4> array_byte;
		array_byte = get_array_byte(tx_vin.second);
		std::copy(std::begin(array_byte), std::end(array_byte), std::back_inserter(tx_vin_vout));
	}
	for(auto &tx_vout: tx.m_vout){
		std::copy(std::begin(tx_vout.first), std::end(tx_vout.first), std::back_inserter(tx_vin_vout));
		std::array<std::byte, 4> array_byte;
		array_byte = get_array_byte(tx_vout.second);
		std::copy(std::begin(array_byte), std::end(array_byte), std::back_inserter(tx_vin_vout));
	}
	crypto_generichash(reinterpret_cast<unsigned char*>(tx.m_txid.data()), tx.m_txid.size(), reinterpret_cast<const unsigned char*>(tx_vin_vout.data()), tx_vin_vout.size(), nullptr, 0);
	std::array<std::byte, crypto_sign_BYTES> sign_txid;
	crypto_sign_detached(reinterpret_cast<unsigned char*>(sign_txid.data()), nullptr, reinterpret_cast<const unsigned char*>(tx.m_txid.data()), tx.m_txid.size(), reinterpret_cast<const unsigned char*>(m_sk.data()));
	tx.m_sign_txid.emplace(m_pk,sign_txid);
	return tx;
}

transaction wallet::generate_tx(uint32_t amount) {
	transaction tx;
	tx.m_type = transactiontype::generate;
	tx.m_vout.emplace(m_wallet_address, amount);
	std::vector<std::byte>txs_vout;
	for(auto &tx_vout: tx.m_vout){
		std::copy(std::begin(tx_vout.first), std::end(tx_vout.first), std::back_inserter(txs_vout));
		std::array<std::byte, 4> array_byte;
		array_byte = get_array_byte(tx_vout.second);
		std::copy(std::begin(array_byte), std::end(array_byte), std::back_inserter(txs_vout));
	}
	crypto_generichash(reinterpret_cast<unsigned char*>(tx.m_txid.data()), tx.m_txid.size(), reinterpret_cast<const unsigned char*>(txs_vout.data()), txs_vout.size(), nullptr, 0);
	std::array<std::byte, crypto_sign_BYTES> sign_txid;
	crypto_sign_detached(reinterpret_cast<unsigned char*>(sign_txid.data()), nullptr, reinterpret_cast<const unsigned char*>(tx.m_txid.data()), tx.m_txid.size(), reinterpret_cast<const unsigned char*>(m_sk.data()));
	tx.m_sign_txid.emplace(m_pk,sign_txid);
	return tx;
}

wallet::wallet(){
	crypto_sign_keypair(reinterpret_cast<unsigned char*>(m_pk.data()), reinterpret_cast<unsigned char*>(m_sk.data()));
	m_wallet_address = get_new_address();
}
