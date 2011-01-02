//ChannelPacket.cpp
//Class file for ChannelPacket object

#include "ChannelPacket.h"

using namespace FDSim;
using namespace std;

ChannelPacket::ChannelPacket(ChannelPacketType packtype, uint64_t physicalAddr, uint page_num, uint block_num, uint plane_num, uint die_num, uint package_num, void *dat)
{
	physicalAddress = physicalAddr;
	busPacketType = packtype;
	data = dat;
	page = page_num;
	block = block_num;
	plane = plane_num;
	die = die_num;
	package = package_num;
}

ChannelPacket::ChannelPacket() {}

void ChannelPacket::print(uint64_t currentClockCycle){
	if (this == NULL)
		return;


	//simple for now
	switch (busPacketType){
		case(READ):
			PRINT("Cycle: "<<currentClockCycle<<" Type: READ addr: "<<physicalAddress<<" die: "<<die<<" plane: "<<plane<<" block: "<<block<<" page: "<<page<<" data: "<<data);
			break;
		case(WRITE):
			PRINT("Cycle: "<<currentClockCycle<<" Type: WRITE addr: "<<physicalAddress<<" die: "<<die<<" plane: "<<plane<<" block: "<<block<<" page: "<<page<<" data: "<<data);
			break;
		case (ERASE):
			PRINT("Cycle: "<<currentClockCycle<<" Type: ERASE addr: "<<physicalAddress<<" die: "<<die<<" plane: "<<plane<<" block: "<<block<<" page: "<<page<<" data: "<<data);
			break;
		case(DATA):
			PRINT("Cycle: "<<currentClockCycle<<" Type: DATA addr: "<<physicalAddress<<" die: "<<die<<" plane: "<<plane<<" block: "<<block<<" page: "<<page<<" data: "<<data);
			break;
		default:
			PRINT("Invalid busPacket type");
			break;
	}


}

void ChannelPacket::printData(const void *data) 
{
	if (data == NULL) 
	{
		PRINTN("NO DATA");
		return;
	}
	PRINTN("'" << hex);
	for (int i=0; i < 4; i++) 
	{
		PRINTN(((uint64_t *)data)[i]);
	}
	PRINTN("'" << dec);
}
