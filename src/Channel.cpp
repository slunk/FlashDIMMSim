//Channel.cpp
//class file for channel class
//no contention modeled yet
#include "Channel.h"
#include "ChannelPacket.h"
#include "Controller.h"

using namespace FDSim;

Channel::Channel(void){
	status = CHANNEL_BUSY;

void Channel::attachDie(Die *d){
	dies.push_back(d);
}

void Channel::attachController(Controller *c){
	controller= c;
}

int Channel::obtainChannel(uint sender){
	if (status == CHANNEL_FREE){
		status = CHANNEL_BUSY;
		owner = sender;
		return 1;
	}
	return 0;
}

int Channel::releaseChannel(uint sender){
	if (status == CHANNEL_BUSY and sender == owner){
		status = CHANNEL_FREE;
		return 1;
	}
	ERROR("Invalid attempt to release channel from "<<sender);
	return 0;
}

int Channel::hasChannel(uint sender){
	if (status == CHANNEL_BUSY and sender == owner)
		return 1;
	return 0;
}

void Channel::sendToDie(uint die_num, ChannelPacket *busPacket){
	dies[die_num]->receiveFromChannel(busPacket);
}

void Channel::sendToController(ChannelPacket *busPacket){
	controller->receiveFromChannel(busPacket);
}
