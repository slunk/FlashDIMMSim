//Plane.cpp
//class file for plane object
//

#include "Plane.h"

using namespace FDSim;
using namespace std;

Plane::Plane(void){
	dataReg= NULL;
	cacheReg= NULL;
}

void Plane::read(ChannelPacket *busPacket){
	if (blocks.find(busPacket->block) != blocks.end()){
		busPacket->data= blocks[busPacket->block].read(busPacket->page);
		busPacket->busPacketType= DATA;
		dataReg= busPacket;
	} else{
		DEBUG("Invalid read: Block "<<busPacket->block<<" hasn't been written to");
		dataReg= new ChannelPacket();//garbage packet... might be better to set values
	}
}

void Plane::write(ChannelPacket *busPacket){
	if (blocks.find(busPacket->block) == blocks.end())
		blocks[busPacket->block] = Block(busPacket->block);
	blocks[busPacket->block].write(busPacket->page, dataReg->data);
}

void Plane::erase(ChannelPacket *busPacket){
	if (blocks.find(busPacket->block) != blocks.end()){
		blocks[busPacket->block].erase();
		blocks.erase(busPacket->block);
	}
}


void Plane::storeInData(ChannelPacket *busPacket){
	dataReg= busPacket;
}

ChannelPacket *Plane::readFromData(void){
	return dataReg;
}
