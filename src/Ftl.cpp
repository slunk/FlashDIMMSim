//Ftl.cpp
//class file for ftl
//
#include "Ftl.h"
#include "ChannelPacket.h"
#include <cmath>

using namespace FDSim;
using namespace std;

Ftl::Ftl(void){
	offset = log2(PAGE_SIZE * 1024);
	pageBitWidth = log2(PAGES_PER_BLOCK);
	blockBitWidth = log2(BLOCKS_PER_PLANE);
	planeBitWidth = log2(PLANES_PER_DIE);
	dieBitWidth = log2(DIES_PER_PACKAGE);
	packageBitWidth = log2(NUM_PACKAGES);
}

//dumb direct mapping for now
ChannelPacket *Ftl::translate(ChannelPacketType type, FlashTransaction &trans){
	uint package, die, plane, block, page;
	uint64_t tempA, tempB, physicalAddress= trans.address;

	if (trans.address > TOTAL_SIZE*1024 - 1 || trans.address < 0){
		ERROR("Inavlid address in Ftl");
		exit(1);
	}

	physicalAddress = physicalAddress >> offset;
	
	tempA = physicalAddress;
	physicalAddress = physicalAddress >> packageBitWidth;
	tempB = physicalAddress << packageBitWidth;
	package = tempA ^ tempB;

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

		
	//cout<<package<<" "<<die<<" "<<plane<<" "<<block<<" "<<" "<<page<<endl;

	return new ChannelPacket(type, trans.address, page, block, plane, die, package, trans.data);
}

//garbage collection here eventually?
void Ftl::update(void){}
