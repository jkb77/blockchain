#include <gtest/gtest.h>
#include "/home/kuba/BC/src/wallet.hpp"
#include "/home/kuba/BC/src/mempool.hpp"
#include "/home/kuba/BC/src/utxo.hpp"
#include "/home/kuba/BC/src/blockchain.hpp"
#include "/home/kuba/BC/src/miner.hpp"

TEST(wallet, make_money) {
	wallet billfold;

	auto tx = billfold.generate_tx(100);

	auto vout_amount = tx.m_vout.begin()->second;

	EXPECT_EQ(vout_amount, 100);
	EXPECT_TRUE(tx.m_vin.empty());
}

TEST(wallet, mempool_tx) {

	wallet billfold1;
	wallet billfold2;
	mempool mem;
	blockchain blocks;
	utxo ux;
	miner mine(ux);
	block bl;

	auto tx1 = billfold1.generate_tx(100);
	auto tx2 = billfold2.generate_tx(100);

	mem.m_transactions.push_back(tx1);
	mem.m_transactions.push_back(tx2);

	auto tx_1 = mem.m_transactions[0];
	auto vout_amount1 = tx_1.m_vout.begin()->second;
	EXPECT_EQ(vout_amount1, 100);
	EXPECT_TRUE(tx_1.m_vin.empty());
	auto tx_2 = mem.m_transactions[1];
	auto vout_amount2 = tx_2.m_vout.find(billfold2.get_address())->second;
	EXPECT_EQ(vout_amount2, 100);
	EXPECT_TRUE(tx_2.m_vin.empty());

	for (auto &tx: mem.m_transactions) {
		mine.get_tx(tx);
	}

	bl = mine.mine_genesisblock();
	auto pk = mine.get_pk();
	blocks.add_block(bl, pk, ux);

	auto address_billfold1 = billfold1.get_address();
	auto address_billfold2 = billfold2.get_address();
	auto vout_address1 = tx_1.m_vout.begin()->first;
	auto money_bill1 = ux.get_amount(vout_address1);
	auto money_bill2 = ux.get_amount(address_billfold2);
	EXPECT_EQ(money_bill1, 100);
	EXPECT_EQ(money_bill2, 100);

	mem.m_transactions.erase(mem.m_transactions.begin(), mem.m_transactions.end());

	auto send_1 = billfold1.send_to_address(address_billfold2, 10);
	auto vout_amount_1 = send_1.m_vout.begin()->second;
	EXPECT_EQ(vout_amount_1, 10);

	mem.m_transactions.push_back(send_1);
	auto tx_3 = mem.m_transactions[0];
	auto vout_amount_1a = tx_3.m_vout.begin()->second;
	auto vin_amount_1a = tx_3.m_vin.begin()->second;
	EXPECT_EQ(vout_amount_1a, 10);
	EXPECT_EQ(vin_amount_1a, 10);

	auto send_2 = billfold2.send_to_address(address_billfold1, 1);
	auto vout_amount_2 = send_2.m_vout.begin()->second;
	EXPECT_EQ(vout_amount_2, 1);

	mem.m_transactions.push_back(send_2);
	auto tx_4 = mem.m_transactions[1];
	auto vout_amount_2a = tx_4.m_vout.begin()->second;
	auto vin_amount_2a = tx_4.m_vin.begin()->second;
	EXPECT_EQ(vout_amount_2a, 1);
	EXPECT_EQ(vin_amount_2a, 1);

	for (auto &tx: mem.m_transactions) {
		mine.get_tx(tx);
	}
	auto prev_block=blocks.get_last_block();
	bl = mine.mine_block(prev_block);
	pk = mine.get_pk();
	blocks.add_block(bl, pk, ux);

	auto money_bill1_end = ux.get_amount(address_billfold1);
	auto money_bill2_end = ux.get_amount(address_billfold2);
	EXPECT_EQ(money_bill1_end, 91);
	EXPECT_EQ(money_bill2_end, 109);
}
