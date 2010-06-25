//Controller.cpp
//Class files for controller

#include "Controller.h"
#include "Ssd.h"

using namespace SSDSim;

Controller::Controller(Ssd* parent){
	int i, j;

	parentSsd= parent;

	channelXferCyclesLeft= vector<uint>(NUM_PACKAGES, 0);

	packetQueues= vector<queue <BusPacket *> >(NUM_PACKAGES, queue<BusPacket *>());
	outgoingPackets= vector<BusPacket *>(NUM_PACKAGES, NULL);

	currentClockCycle= 0;
}

void Controller::attachPackages(vector<Package> *packages){
	this->packages= packages;
}

bool Controller::addTransaction(Transaction &trans){
	trans.timeAdded= currentClockCycle;
	transactionQueue.push(trans);
	return true;
}

void Controller::returnReadData(const Transaction  &trans){
	if(parentSsd->ReturnReadData!=NULL){
		(*parentSsd->ReturnReadData)(parentSsd->systemID, trans.address, currentClockCycle);
	}
}

void Controller::receiveFromChannel(BusPacket *busPacket){
	returnTransaction.push_back(Transaction(RETURN_DATA, busPacket->physicalAddress, busPacket->data));
	delete(busPacket);
}

void Controller::update(void){
	//ffs this is gonna suck
	int i;
	
	for (i= 0; i < outgoingPackets.size(); i++){
		if (!(outgoingPackets[i] != NULL)){
			channelXferCyclesLeft[i]--;
			if (channelXferCyclesLeft[i] == 0){
				(*packages)[outgoingPackets[i]->package].dies[outgoingPackets[i]->die].receiveFromChannel(outgoingPackets[i]);
				//packages[outgoingPackets[i]->package].channel->releaseChannel();
				outgoingPackets[i]= NULL;
			}
		}
	}
	
	//Look through queues and send oldest packets to the appropriate channel
	for (i= 0; i < packetQueues.size(); i++){
		if (!packetQueues[i].empty() && outgoingPackets[i]==NULL){
			outgoingPackets[i]= packetQueues[i].front();
			packetQueues[i].pop();
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
