//Die.cpp
//class file for die class

#include "Die.h"
#include "Channel.h"
#include "Controller.h"
#include "FlashDIMM.h"

using namespace FDSim;
using namespace std;

Die::Die(FlashDIMM *parent){
	parentFlashDIMM= parent;

	planes= vector<Plane>(PLANES_PER_DIE, Plane());

	currentCommand= NULL;

	dataCyclesLeft= 0;
	controlCyclesLeft= 0;

	currentClockCycle= 0;

}

void Die::attachToChannel(Channel *chan){
	channel= chan;
}

void Die::receiveFromChannel(BusPacket *busPacket){
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
	//not actually if a single plane is busy (tests if die is busy).
	//only one command gets executed by a die at a time
	//If concurrency between planes gets added (which may be necessary)
	//this function will have to change along with a lot of other stuff...
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
					parentFlashDIMM->numReads++;
					break;
				case WRITE:
					planes[currentCommand->plane].write(currentCommand);
					parentFlashDIMM->numWrites++;
					//call read callback
					if (parentFlashDIMM->WriteDataDone != NULL){
						(*parentFlashDIMM->WriteDataDone)(parentFlashDIMM->systemID, currentCommand->physicalAddress, currentClockCycle);
					}
					break;
				case ERASE:
					planes[currentCommand->plane].erase(currentCommand);
					parentFlashDIMM->numErases++;
					break;
				default:
					break;
			}
			
			//sim output
			//currentCommand->print(currentClockCycle);
			delete(currentCommand);
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
