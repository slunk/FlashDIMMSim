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

	addressMap = std::unordered_map<uint64_t, uint64_t>();
	dirty = vector<vector<bool>>(numBlocks, vector<bool>(PAGES_PER_BLOCK, false));
	used = vector<vector<bool>>(numBlocks, vector<bool>(PAGES_PER_BLOCK, false));
	transactionQueue = list<FlashTransaction>();

	used_page_count = 0;

	controller = c;
}

ChannelPacket *Ftl::translate(ChannelPacketType type, uint64_t vAddr, uint64_t pAddr){
	uint package, die, plane, block, page;
	uint64_t tempA, tempB, physicalAddress = pAddr;

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

	return new ChannelPacket(type, vAddr, pAddr, page, block, plane, die, package, NULL);
}

bool Ftl::addTransaction(FlashTransaction &t){
	transactionQueue.push_back(t);
	return true;
}

void Ftl::update(void){
	uint64_t block, page, start;

	// Decrement block erase counters
	for (std::unordered_map<uint64_t,uint64_t>::iterator it = erase_counter.begin(); it != erase_counter.end(); it++) {

		// Decrement the counter.
		uint64_t cnt = (*it).second;
		cnt--;
		(*it).second = cnt;

		// Check to see if done.
		if (cnt == 0) {
			// Set all the bits in the page to be clean.
			block = (*it).first;
			for (page = 0; page < PAGES_PER_BLOCK; page++) {
				used[block][page] = false;
				used_page_count--;
				dirty[block][page] = false;
			}

			// Remove from erase counter map.
			erase_counter.erase(it);
		}
	}

	if (busy) {
		if (lookupCounter == 0){
			uint64_t vAddr = currentTransaction.address, pAddr;
			bool done = false;
			ChannelPacket *commandPacket, *dataPacket;
			
			switch (currentTransaction.transactionType){
				case DATA_READ:
					if (addressMap.find(vAddr) == addressMap.end()){
						controller->returnReadData(FlashTransaction(RETURN_DATA, vAddr, (void *)0xdeadbeef));
					} else {
						commandPacket = Ftl::translate(READ, vAddr, addressMap[vAddr]);
						controller->addPacket(commandPacket);
					}
					break;
				case DATA_WRITE:
					if (addressMap.find(vAddr) != addressMap.end()){
						dirty[addressMap[vAddr] / (BLOCK_SIZE * 1024)][(addressMap[vAddr] / FLASH_PAGE_SIZE) % PAGES_PER_BLOCK] = true;
					}
					//look for first free physical page starting at the write pointer
					start = FLASH_PAGE_SIZE * PAGES_PER_BLOCK * BLOCKS_PER_PLANE * (plane + PLANES_PER_DIE * (die + NUM_PACKAGES * channel));

					for (block = start / BLOCK_SIZE ; block < TOTAL_SIZE / BLOCK_SIZE && !done; block++)
						for (page = 0 ; page < PAGES_PER_BLOCK  && !done ; page++)
							if (!used[block][page]){
								pAddr = (block * BLOCK_SIZE + page * FLASH_PAGE_SIZE) * 1024;
								used[block][page] = true;
								used_page_count++;
								done = true;
							}

					//if we didn't find a free page after scanning til the end, check the beginning

					if (!done)
						for (block = 0 ; block < start / BLOCK_SIZE && !done ; block++)
							for (page = 0 ; page < PAGES_PER_BLOCK && !done ; page++)
								if (!used[block][page]){
									pAddr = (block * BLOCK_SIZE + page * FLASH_PAGE_SIZE) * 1024;
									used[block][page] = true;
									used_page_count++;
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
					dataPacket = Ftl::translate(DATA, vAddr, pAddr);
					commandPacket = Ftl::translate(WRITE, vAddr, pAddr);
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

				case BLOCK_ERASE:
					// Note: For this command, vAddr refers to the block number to erase.
					erase_counter[vAddr] = 1000000; // Initially hardcoded as 1.5 ms.
					break;
				default:
					ERROR("Transaction in Ftl that isn't a read or write... What?");
					exit(1);
					break;
			}
			transactionQueue.pop_front();
			busy = 0;
			lookupCounter = -1;
		} 
		else
			lookupCounter--;
	} // if busy
	else {
		// Not currently busy.

		if (!transactionQueue.empty()) {
			busy = 1;
			currentTransaction = transactionQueue.front();
			lookupCounter = LOOKUP_TIME;
		}
		else {
			// Check to see if GC needs to run.
			if (checkGC()) {
				// Run the GC.
				runGC();
			}

		}
	
	}

}

bool Ftl::checkGC(void){
	//uint64_t block, page, count = 0;

	// Count the number of blocks with used pages.
	//for (block = 0; block < TOTAL_SIZE / BLOCK_SIZE; block++) {
	//	for (page = 0; page < PAGES_PER_BLOCK; page++) {
	//		if (used[block][page] == true) {
	//			count++;
	//			break;
	//		}
	//	}
	//}
	
	// Return true if more than 70% of blocks are in use and false otherwise.
	if (((float)used_page_count / TOTAL_SIZE) > 0.7)
		return true;
	else
		return false;
}


void Ftl::runGC(void) {
	uint64_t block, page, count, dirty_block=0, dirty_count=0, pAddr, vAddr, tmpAddr;
	FlashTransaction trans;

	// Get the dirtiest block (assumes the flash keeps track of this with an online algorithm).
	for (block = 0; block < TOTAL_SIZE / BLOCK_SIZE; block++) {
		count = 0;
		for (page = 0; page < PAGES_PER_BLOCK; page++) {
			if (dirty[block][page] == true) {
				count++;
			}
		}
		if (count > dirty_count) {
			dirty_count = count;
			dirty_block = block;
		}
	}

	// All used pages in the dirty block, they must be moved elsewhere.
	for (page = 0; page < PAGES_PER_BLOCK; page++) {
		if (used[dirty_block][page] == true && dirty[dirty_block][page] == false) {
			// Compute the physical address to move.
			pAddr = (dirty_block * BLOCK_SIZE + page * FLASH_PAGE_SIZE) * 1024;

			// Do a reverse lookup for the virtual page address.
			// This is slow, but the alternative is maintaining a full reverse lookup map.
			// Another alternative may be to make new FlashTransaction commands for physical address read/write.
			bool found = false;
			for (std::unordered_map<uint64_t, uint64_t>::iterator it = addressMap.begin(); it != addressMap.end(); it++) {
				tmpAddr = (*it).second;
				if (tmpAddr == pAddr) {
					vAddr = (*it).first;
					found = true;
					break;
				}
			}
			assert(found);
			

			// Schedule a read and a write.
			trans = FlashTransaction(DATA_READ, vAddr, NULL);
			addTransaction(trans);
			trans = FlashTransaction(DATA_WRITE, vAddr, NULL);
			addTransaction(trans);
		}
	}

	// Schedule the BLOCK_ERASE command.
	// Note: The address field is just the block number, not an actual byte address.
	trans = FlashTransaction(BLOCK_ERASE, dirty_block, NULL);
	addTransaction(trans);

}

uint64_t Ftl::get_ptr(void) {
	// Return a pointer to the current plane.
	return FLASH_PAGE_SIZE * PAGES_PER_BLOCK * BLOCKS_PER_PLANE * 
			(plane + PLANES_PER_DIE * (die + NUM_PACKAGES * channel));
}

