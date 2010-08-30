#ifndef FDSIMTRANSACTION_H
#define FDSIMTRANSACTION_H

//FlashTransaction.h
//
//Header file for transaction object

#include "FlashConfiguration.h"

using namespace std;

namespace FDSim
{
	enum TransactionType
	{
		DATA_READ, 
		DATA_WRITE,
		RETURN_DATA
	};

	class FlashTransaction
	{
	public:
		//fields
		TransactionType transactionType;
		uint64_t address;
		void *data;
		uint64_t timeAdded;
		uint64_t timeReturned;

		
		//functions
		FlashTransaction(TransactionType transType, uint64_t addr, void *data);
		FlashTransaction();
		
		void print();
	};
}

#endif

