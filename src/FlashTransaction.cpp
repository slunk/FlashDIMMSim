//FlashTransaction.cpp
//
//Class file for transaction object
//	FlashTransaction is considered requests sent from the CPU to 
//	the memory controller (read, write, etc.)...

#include "FlashTransaction.h"

using namespace FDSim;
using namespace std;

FlashTransaction::FlashTransaction(){}

FlashTransaction::FlashTransaction(TransactionType transType, uint64_t addr, void *dat)
{
	transactionType = transType;
	address = addr;
	data = dat;
}

void FlashTransaction::print()
{
	if(transactionType == DATA_READ)
		{
			PRINT("T [Read] [0x" << hex << address << "]" << dec );
		}
	else if(transactionType == DATA_WRITE)
		{
			PRINT("T [Write] [0x" << hex << address << "] [" << dec << data << "]" );
		}
	else if(transactionType == RETURN_DATA)
		{
			PRINT("T [Data] [0x" << hex << address << "] [" << dec << data << "]" );
		}
}

