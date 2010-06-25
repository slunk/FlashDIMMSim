//Ssd.cpp
//Class file for ssd system wrapper

#include "Ssd.h"

using namespace SSDSim;
using namespace std;

Ssd::Ssd(uint id, string dev, string sys, string pwd, string trc){
	/*int i;
	for (i= 0; i < NUM_PACKAGES; i++){
		Package pack;
		pack.channel= new Channel();
		for (j= 0; j < NUM_DIES; j++){
			Die die= Die();
			die.attachToChannel(pack.channel);
			pack.dies.push_back(die);
		}
		packages->push_back(pack);
	}*/
	currentClockCycle= 0;
}

bool Ssd::add(Transaction &trans){
	return true;
}

bool Ssd::addTransaction(bool isWrite, uint64_t addr){
	return true;
}

string Ssd::SetOutputFileName(string tracefilename){
	return "";
}

void Ssd::RegisterCallbacks(Callback_t *readDone, Callback_t *writeDone){
}

void Ssd::printStats(void){
}

void Ssd::update(void){
	step();
}
