#include "transaction.hpp"

bool operator==(const transaction &lhs, const transaction &rhs) {
	if (lhs.m_txid==rhs.m_txid) return true;
	else return false;
}

bool operator!=(const transaction &lhs, const transaction &rhs) {
	if (lhs.m_txid!=rhs.m_txid) return true;
	else return false;
}
