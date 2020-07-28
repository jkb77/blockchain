#ifndef MEMPOOL_HPP
#define MEMPOOL_HPP
#include "transaction.hpp"

struct mempool {
	std::vector<transaction> m_transactions;
};

#endif // MEMPOOL_HPP
