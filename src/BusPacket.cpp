//BusPacket.cpp
//Class file for BusPacket object

#include "BusPacket.h"

using namespace SSDSim;
using namespace std;

BusPacket::BusPacket(BusPacketType packtype, uint64_t physicalAddr, uint page_num, uint block_num, uint plane_num, uint die_num, uint package_num, void *dat)
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

BusPacket::BusPacket() {}

void BusPacket::print(uint64_t currentClockCyle){
	if (this == NULL)
		return;


	//simple for now
	cout<<"Type: "<<busPacketType<<" addr: "<<physicalAddress<<" die: "<<die<<" plane: "<<plane<<" block: "<<block<<" page: "<<page<<" data: "<<data<<endl;
}

void BusPacket::printData(const void *data) 
{
	/*if (data == NULL) 
	{
		PRINTN("NO DATA");
		return;
	}
	PRINTN("'" << hex);
	for (int i=0; i < 4; i++) 
	{
		PRINTN(((uint64_t *)data)[i]);
	}
	PRINTN("'" << dec);*/
}
