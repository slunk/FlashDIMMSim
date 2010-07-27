//Channel.cpp
//class file for channel class
//no contention modeled yet
#include "Channel.h"
#include "ChannelPacket.h"
#include "Controller.h"

using namespace FDSim;

Channel::Channel(void){}

void Channel::attachDie(Die *d){
	dies.push_back(d);
}

void Channel::attachController(Controller *c){
	controller= c;
}

int Channel::obtainChannel(uint sender){
	return 1;
}

int Channel::releaseChannel(uint sender){
	return 1;
}

int Channel::hasChannel(uint sender){
	return 1;
}

void Channel::sendToDie(uint die_num, ChannelPacket *busPacket){
	dies[die_num]->receiveFromChannel(busPacket);
}

void Channel::sendToController(ChannelPacket *busPacket){
	controller->receiveFromChannel(busPacket);
}
