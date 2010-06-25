//Controller.cpp
//Class files for controller

#include "Controller.h"
#include "Ssd.h"

using namespace SSDSim;

Controller::Controller(Ssd* parent){
	int i, j;

	parentSsd= parent;

	channelXferCyclesLeft= vector<uint>(NUM_PACKAGES, 0);

	channelQueues= vector<queue <BusPacket *> >(NUM_PACKAGES, queue<BusPacket *>());
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
	int i;
	
	//Check for commands/data on a channel. If there is, see if it is done on channel
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
	for (i= 0; i < channelQueues.size(); i++){
		if (!channelQueues[i].empty() && outgoingPackets[i]==NULL){
			//if we can get the channel (channel contention not implemented yet)
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

	//Look for new transactions. If there are any, translate their address, make buspackets, and place in appropriate channel queue
	//Everything past his point will probably need some drasic changes in future iterations
	while (transactionQueue.size() > 0){//This is probably a terrible way to do this
		switch (transactionQueue.front().transactionType){
			case DATA_READ:{
				BusPacket *readPacket= ftl.translate(READ, transactionQueue.front());
				channelQueues[readPacket->package].push(readPacket);
				}
				break;
			case DATA_WRITE:{
				BusPacket *dataPacket= ftl.translate(DATA, transactionQueue.front());
				BusPacket *writePacket= new BusPacket(WRITE, dataPacket->physicalAddress, dataPacket->page, dataPacket->block, dataPacket->plane, dataPacket->die, dataPacket->package, dataPacket->data); 
				channelQueues[writePacket->package].push(dataPacket);
				channelQueues[writePacket->package].push(writePacket);
				}
				break;
			default:
				ERROR("Invalid transaction type from hybrid controller: "<<transactionQueue.front().transactionType);
				exit(1);
				break;
		}
		transactionQueue.pop();
	}
	
	//See if any read data is ready to return
	while (!returnTransaction.empty()){
		returnReadData(returnTransaction.front());
		returnTransaction.pop();
	}
}
