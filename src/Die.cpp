//Die.cpp
//class file for die class

#include "Die.h"
#include "Channel.h"

using namespace SSDSim;
using namespace std;

Die::Die(int num_planes){
	int x;
	for (x= 0; x < num_planes; x++){
		planes.push_back(new Plane());
	}

	dataCyclesLeft= 0;
	controlCyclesLeft= 0;
}

void Die::attachToChannel(Channel *chan){
	this->channel= channel;
}

void Die::receiveFromChannel(BusPacket *busPacket){
	if (busPacket->busPacketType == DATA){
		planes[busPacket->plane]->storeInData(*busPacket);
	} else{
		commands.push(busPacket);
	}
}

void Die::update(void){
	if (currentCommand != NULL){
		if (controlCyclesLeft == 0){
			switch (currentCommand->busPacketType){
				case READ:
					planes[currentCommand->plane]->read(*currentCommand);
					//plane needs changes
					//returnDataPackets.push(planes[currentCommand->plane]->readFromData());
					break;
				case WRITE:
					planes[currentCommand->plane]->write(*currentCommand);
					break;
				case ERASE:
					planes[currentCommand->plane]->erase(*currentCommand);
					break;
				default:
					break;
			}
			currentCommand= NULL;
			//look at next command
			if (!commands.empty()){
				currentCommand= commands.front();
				commands.pop();
				switch (currentCommand->busPacketType){
					case READ:
						controlCyclesLeft= READ_TIME;
						break;
					case WRITE:
						controlCyclesLeft= WRITE_TIME;
						break;
					case ERASE:
						controlCyclesLeft= ERASE_TIME;
						break;
					default:
						break;
				}
			}	
		} else
			controlCyclesLeft--;
	}

	if (!returnDataPackets.empty()){
		//return the data
		if (dataCyclesLeft == 0){
			channel->sendToController(returnDataPackets.front());
			returnDataPackets.pop();
			if (!returnDataPackets.empty())
				dataCyclesLeft= DATA_TIME;
		}
		dataCyclesLeft--;
	}
}
