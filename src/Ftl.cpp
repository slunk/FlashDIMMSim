//Ftl.cpp
//class file for ftl
//
#include "Ftl.h"
#include "ChannelPacket.h"
#include <cmath>

using namespace FDSim;
using namespace std;

Ftl::Ftl(Controller *c){
	int numBlocks = NUM_PACKAGES * DIES_PER_PACKAGE * PLANES_PER_DIE * BLOCKS_PER_PLANE;

	offset = log2(FLASH_PAGE_SIZE * 1024);
	pageBitWidth = log2(PAGES_PER_BLOCK);
	blockBitWidth = log2(BLOCKS_PER_PLANE);
	planeBitWidth = log2(PLANES_PER_DIE);
	dieBitWidth = log2(DIES_PER_PACKAGE);
	packageBitWidth = log2(NUM_PACKAGES);

	channel = 0;
	die = 0;
	plane = 0;
	lookupCounter = 0;

	busy = 0;

	addressMap = unordered_map<uint64_t, uint64_t>();
	dirty = vector<vector<bool>>(numBlocks, vector<bool>(PAGES_PER_BLOCK, false));
	used = vector<vector<bool>>(numBlocks, vector<bool>(PAGES_PER_BLOCK, false));
	transactionQueue = list<FlashTransaction>();

	controller = c;
}

ChannelPacket *Ftl::translate(ChannelPacketType type, uint64_t addr){
	uint package, die, plane, block, page;
	uint64_t tempA, tempB, physicalAddress = addr;

	if (physicalAddress > TOTAL_SIZE*1024 - 1 || physicalAddress < 0){
		ERROR("Inavlid address in Ftl: "<<physicalAddress);
		exit(1);
	}

	physicalAddress = physicalAddress >> offset;

	tempA = physicalAddress;
	physicalAddress = physicalAddress >> pageBitWidth;
	tempB = physicalAddress << pageBitWidth;
	page = tempA ^ tempB;

	tempA = physicalAddress;
	physicalAddress = physicalAddress >> blockBitWidth;
	tempB = physicalAddress << blockBitWidth;
	block = tempA ^ tempB;

	tempA = physicalAddress;
	physicalAddress = physicalAddress >> planeBitWidth;
	tempB = physicalAddress << planeBitWidth;
	plane = tempA ^ tempB;

	tempA = physicalAddress;
	physicalAddress = physicalAddress >> dieBitWidth;
	tempB = physicalAddress << dieBitWidth;
	die = tempA ^ tempB;
	
	tempA = physicalAddress;
	physicalAddress = physicalAddress >> packageBitWidth;
	tempB = physicalAddress << packageBitWidth;
	package = tempA ^ tempB;

//cout<<package<<" "<<die<<" "<<plane<<" "<<block<<" "<<" "<<page<<endl;

	return new ChannelPacket(type, addr, page, block, plane, die, package, NULL);
}

bool Ftl::addTransaction(FlashTransaction &t){
	transactionQueue.push_back(t);
	return true;
}

void Ftl::update(void){
	uint64_t block, page, start;

	if (!busy && !transactionQueue.empty()){
		busy = 1;
		currentTransaction = transactionQueue.front();
		lookupCounter = LOOKUP_TIME;
	}
	
	if (lookupCounter == 0){
		uint64_t vAddr = currentTransaction.address, pAddr;
		bool done = false;
		ChannelPacket *commandPacket, *dataPacket;

		switch (currentTransaction.transactionType){
			case DATA_READ:
				if (addressMap.find(vAddr) == addressMap.end()){
					controller->returnReadData(FlashTransaction(RETURN_DATA, 0, (void *)0xdeadbeef));
				} else {
					commandPacket = Ftl::translate(READ, addressMap[vAddr]);
					controller->addPacket(commandPacket);
				}
				break;
			case DATA_WRITE:
				if (addressMap.find(vAddr) != addressMap.end()){
					dirty[addressMap[vAddr] / BLOCK_SIZE][(addressMap[vAddr] / FLASH_PAGE_SIZE) % PAGES_PER_BLOCK] = true;
				}
				//look for first free physical page starting at the write pointer
				start = FLASH_PAGE_SIZE * PAGES_PER_BLOCK * BLOCKS_PER_PLANE * (plane + PLANES_PER_DIE * (die + NUM_PACKAGES * channel));//yuck!

				for (block = start / BLOCK_SIZE ; block < TOTAL_SIZE / BLOCK_SIZE && !done; block++)
					for (page = 0 ; page < PAGES_PER_BLOCK  && !done ; page++)
						if (!used[block][page]){
							pAddr = (block * BLOCK_SIZE + page * FLASH_PAGE_SIZE) * 1024;
							used[block][page] = true;
							done = true;
						}

				//if we didn't find a free page after scanning til the end, check the beginning

				if (!done)
					for (block = 0 ; block < start / BLOCK_SIZE && !done ; block++)
						for (page = 0 ; page < PAGES_PER_BLOCK && !done ; page++)
							if (!used[block][page]){
								pAddr = (block * BLOCK_SIZE + page * FLASH_PAGE_SIZE) * 1024;
								used[block][page] = true;
								done = true;
							}

				if (!done){
					// TODO: Call GC
					ERROR("No free pages? GC needs some work.");
					exit(1);
				} else {
					addressMap[vAddr] = pAddr;
				}
				//send write to controller
				dataPacket = Ftl::translate(DATA, pAddr);
				commandPacket = Ftl::translate(WRITE, pAddr);
				controller->addPacket(dataPacket);
				controller->addPacket(commandPacket);
				//update "write pointer"
				channel = (channel + 1) % NUM_PACKAGES;
				if (channel == 0){
					die = (die + 1) % DIES_PER_PACKAGE;
					if (die == 0)
						plane = (plane + 1) % PLANES_PER_DIE;
				}
				break;
			default:
				ERROR("Transaction in Ftl that isn't a read or write... What?");
				exit(1);
				break;
		}
		transactionQueue.pop_front();
		busy = 0;
		lookupCounter = -1;
	} else if (busy)
		lookupCounter--;
}
