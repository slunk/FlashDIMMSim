//Channel.cpp
//class file for channel class
//no contention modeled yet
#include "Channel.h"
#include "ChannelPacket.h"
#include "Controller.h"

using namespace FDSim;

Channel::Channel(void){
	sender = -1;
}

void Channel::attachDie(Die *d){
	dies.push_back(d);
}

void Channel::attachController(Controller *c){
	controller= c;
}

int Channel::obtainChannel(uint s, SenderType t, ChannelPacket *p){
	if (sender != -1 || (t == CONTROLLER && dies[p->die]->isDieBusy(p->plane))){
		return 0;
	}
	type = t;
	sender = (int) s;
	return 1;
}

int Channel::releaseChannel(SenderType t, uint s){
	if (t == type && sender == (int) s){
		sender = -1;
		return 1;
	}
	return 0;
}

int Channel::hasChannel(SenderType t, uint s){
	if (t == type && sender == (int) s)
		return 1;
	return 0;
}

void Channel::sendToDie(ChannelPacket *busPacket){
	dies[busPacket->die]->receiveFromChannel(busPacket);
}

void Channel::sendToController(ChannelPacket *busPacket){
	controller->receiveFromChannel(busPacket);
}
