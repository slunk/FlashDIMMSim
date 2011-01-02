//Controller.cpp
//Class files for controller

#include "Controller.h"
#include "FlashDIMM.h"

using namespace FDSim;

Controller::Controller(FlashDIMM* parent){
	parentFlashDIMM= parent;

	channelXferCyclesLeft= vector<uint>(NUM_PACKAGES, 0);

	channelQueues= vector<queue <ChannelPacket *> >(NUM_PACKAGES, queue<ChannelPacket *>());
	outgoingPackets= vector<ChannelPacket *>(NUM_PACKAGES, NULL);

	currentClockCycle= 0;
}

void Controller::attachPackages(vector<Package> *packages){
	this->packages= packages;
}

bool Controller::addTransaction(FlashTransaction &trans){
	trans.timeAdded= currentClockCycle;
	transactionQueue.push_back(trans);
	return true;
}

void Controller::returnReadData(const FlashTransaction  &trans){
	if(parentFlashDIMM->ReturnReadData!=NULL){
		(*parentFlashDIMM->ReturnReadData)(parentFlashDIMM->systemID, trans.address, currentClockCycle);
	}
	parentFlashDIMM->numReads++;
}

void Controller::receiveFromChannel(ChannelPacket *busPacket){
	returnTransaction.push_back(FlashTransaction(RETURN_DATA, busPacket->physicalAddress, busPacket->data));
	delete(busPacket);
}

bool Controller::addPacket(ChannelPacket *p){
	channelQueues[p->package].push(p);
	return true;
}

void Controller::update(void){
	uint i;
	
	//Check for commands/data on a channel. If there is, see if it is done on channel
	for (i= 0; i < outgoingPackets.size(); i++){
		if (outgoingPackets[i] != NULL && (*packages)[outgoingPackets[i]->package].channel->hasChannel(CONTROLLER, 0)){

			channelXferCyclesLeft[i]--;
			if (channelXferCyclesLeft[i] == 0){
				(*packages)[outgoingPackets[i]->package].channel->sendToDie(outgoingPackets[i]);
				(*packages)[outgoingPackets[i]->package].channel->releaseChannel(CONTROLLER, 0);
				outgoingPackets[i]= NULL;
			}
		}
	}
	
	//Look through queues and send oldest packets to the appropriate channel
	for (i= 0; i < channelQueues.size(); i++){
		if (!channelQueues[i].empty() && outgoingPackets[i]==NULL){
			//if we can get the channel (channel contention not implemented yet)
			if ((*packages)[i].channel->obtainChannel(0, CONTROLLER, channelQueues[i].front())){
				outgoingPackets[i]= channelQueues[i].front();
				channelQueues[i].pop();
				switch (outgoingPackets[i]->busPacketType){
					case DATA:
						channelXferCyclesLeft[i]= DATA_TIME;
						break;
					default:
						channelXferCyclesLeft[i]= COMMAND_TIME;
						break;
				}
			}
		}
	}
		
	//See if any read data is ready to return
	while (!returnTransaction.empty()){
		//call return callback
		returnReadData(returnTransaction.back());
		returnTransaction.pop_back();
	}
}
