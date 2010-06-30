//Plane.cpp
//class file for plane object
//

#include "Plane.h"

using namespace SSDSim;
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
		cerr<<"Invalid read: Block hasn't been written to\n";
		dataReg= NULL;
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
