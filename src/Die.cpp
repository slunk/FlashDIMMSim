//Die.cpp
//class file for die class

#include "Die.h"
#include "Channel.h"
#include "Controller.h"
#include "Ssd.h"

using namespace SSDSim;
using namespace std;

Die::Die(Ssd *parent){
	parentSsd= parent;

	planes= vector<Plane>(NUM_PLANES, Plane());

	currentCommand= NULL;

	dataCyclesLeft= 0;
	controlCyclesLeft= 0;

	currentClockCycle= 0;

}

void Die::attachToChannel(Channel *chan){
	channel= chan;
}

void Die::receiveFromChannel(BusPacket *busPacket){
	//busPacket->print(currentClockCycle);
	if (busPacket->busPacketType == DATA){
		planes[busPacket->plane].storeInData(busPacket);
	} else if (currentCommand == NULL) {
		commands.push(busPacket);
	} else{
		ERROR("Die is busy\n");
		exit(1);
	}
}

int Die::isPlaneBusy(BusPacket *busPacket){
	if (currentCommand == NULL){
		return 0;
	}
	return 1;
}

void Die::update(void){
	if (currentCommand != NULL){
		if (controlCyclesLeft == 0){
			switch (currentCommand->busPacketType){
				case READ:
					planes[currentCommand->plane].read(currentCommand);
					returnDataPackets.push(planes[currentCommand->plane].readFromData());
					parentSsd->numReads++;
					PRINT("READ");
					break;
				case WRITE:
					planes[currentCommand->plane].write(currentCommand);
					parentSsd->numWrites++;
					//the following two lines are terrible. TODO: edit classes so that this isn't so damn messy
					if (channel->controller->parentSsd->WriteDataDone != NULL){
						(*channel->controller->parentSsd->WriteDataDone)(channel->controller->parentSsd->systemID, currentCommand->physicalAddress, currentClockCycle);
					}
					PRINT("WRITE");
					break;
				case ERASE:
					planes[currentCommand->plane].erase(currentCommand);
					parentSsd->numErases++;
					PRINT("ERASE");
					break;
				default:
					break;
			}
			
			//sim output
			currentCommand->print(currentClockCycle);
			currentCommand= NULL;
		} 
		controlCyclesLeft--;
	} else{
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
	}

	//using channel without contention for now
	if (!returnDataPackets.empty()){
		//return the data
		if (dataCyclesLeft == 0){
			channel->sendToController(returnDataPackets.front());
			returnDataPackets.pop();
			if (!returnDataPackets.empty())
				if (channel->obtainChannel(0));
					dataCyclesLeft= DATA_TIME;
		}
		dataCyclesLeft--;
	}
}
