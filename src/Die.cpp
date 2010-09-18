//Die.cpp
//class file for die class

#include "Die.h"
#include "Channel.h"
#include "Controller.h"
#include "FlashDIMM.h"

using namespace FDSim;
using namespace std;

Die::Die(FlashDIMM *parent, uint idNum){
	id = idNum;
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

void Die::receiveFromChannel(ChannelPacket *busPacket){
	 if (busPacket->busPacketType == DATA){
		 planes[busPacket->plane].storeInData(busPacket);
	 } else if (currentCommand == NULL) {
		 currentCommand = busPacket;
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
	 } else{
		 ERROR("Die is busy");
		 exit(1);
	 }
}

int Die::isDieBusy(void){
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
					 break;
				 case WRITE:
					 planes[currentCommand->plane].write(currentCommand);
					 parentFlashDIMM->numWrites++;
					 //call write callback
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
			 currentCommand= NULL;
		 }
		 controlCyclesLeft--;
	 }

	//using channel without contention for now
	if (!returnDataPackets.empty()){
		if (channel->hasChannel(DIE, id)){
			if (dataCyclesLeft == 0){
				channel->sendToController(returnDataPackets.front());
				channel->releaseChannel(DIE, id);
				returnDataPackets.pop();
			}
			dataCyclesLeft--;
		} else
			if (channel->obtainChannel(id, DIE, 0))
				dataCyclesLeft= DATA_TIME;
	}
}
