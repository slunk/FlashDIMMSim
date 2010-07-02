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

void Plane::read(BusPacket *busPacket){
	if (blocks.find(busPacket->block) != blocks.end()){
		busPacket->data= blocks[busPacket->block].read(busPacket->page);
		busPacket->busPacketType= DATA;
		dataReg= busPacket;
	} else{
		ERROR("Invalid read: Block "<<busPacket->block<<" hasn't been written to");
		dataReg= new BusPacket();//garbage packet... might be better to set values
	}
}

void Plane::write(BusPacket *busPacket){
	if (blocks.find(busPacket->block) == blocks.end())
		blocks[busPacket->block] = Block(busPacket->block);
	blocks[busPacket->block].write(busPacket->page, dataReg->data);
}

void Plane::erase(BusPacket *busPacket){
	if (blocks.find(busPacket->block) != blocks.end()){
		blocks[busPacket->block].erase();
		blocks.erase(busPacket->block);
	}
}


void Plane::storeInData(BusPacket *busPacket){
	dataReg= busPacket;
}

BusPacket *Plane::readFromData(void){
	return dataReg;
}
