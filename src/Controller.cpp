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
}

void Controller::receiveFromChannel(ChannelPacket *busPacket){
	returnTransaction.push_back(FlashTransaction(RETURN_DATA, busPacket->physicalAddress, busPacket->data));
	delete(busPacket);
}

void Controller::update(void){
	uint i;
	
	//Check for commands/data on a channel. If there is, see if it is done on channel
	for (i= 0; i < outgoingPackets.size(); i++){
		if (outgoingPackets[i] != NULL && !(*packages)[outgoingPackets[i]->package].dies[outgoingPackets[i]->die]->isPlaneBusy(outgoingPackets[i])){

			channelXferCyclesLeft[i]--;
			if (channelXferCyclesLeft[i] == 0){
				(*packages)[outgoingPackets[i]->package].dies[outgoingPackets[i]->die]->receiveFromChannel(outgoingPackets[i]);
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
		//transactionQueue.front().print();
		switch (transactionQueue.front().transactionType){
			case DATA_READ:{
				ChannelPacket *readPacket= ftl.translate(READ, transactionQueue.front());
				channelQueues[readPacket->package].push(readPacket);
				}
				break;
			case DATA_WRITE:
				{
				ChannelPacket *dataPacket= ftl.translate(DATA, transactionQueue.front());
				ChannelPacket *writePacket= new ChannelPacket(WRITE, dataPacket->physicalAddress, dataPacket->page, dataPacket->block, dataPacket->plane, dataPacket->die, dataPacket->package, dataPacket->data); 
				channelQueues[writePacket->package].push(dataPacket);
				channelQueues[writePacket->package].push(writePacket);
				}
				break;
			default:
				ERROR("Invalid transaction type from hybrid controller: "<<transactionQueue.front().transactionType);
				exit(1);
				break;
		}
		transactionQueue.erase(transactionQueue.begin());
	}
	
	//See if any read data is ready to return
	while (!returnTransaction.empty()){
		//call return callback
		returnReadData(returnTransaction.back());
		returnTransaction.pop_back();
	}
}
