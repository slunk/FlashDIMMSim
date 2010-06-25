//Controller.cpp
//Class files for controller

#include "Controller.h"

using namespace SSDSim;

Controller::Controller(Ssd* parent){
	int i, j;

	parentSsd= parent;

	for (i= 0; i < NUM_PACKAGES; i++){
		Package pack;
		pack.channel= new Channel();
		for (j= 0; j < NUM_DIES; j++){
			Die die= Die();
			die.attachToChannel(pack.channel);
			pack.dies.push_back(die);
		}
		packages.push_back(pack);
	}

	channelXferCyclesLeft= vector<uint>(NUM_PACKAGES, 0);

	outgoingPackets= vector<std::queue <BusPacket *> >(NUM_PACKAGES, queue<BusPacket *>());

	currentClockCycle= 0;
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
}

void Controller::update(void){
}
