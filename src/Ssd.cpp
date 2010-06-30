//Ssd.cpp
//Class file for ssd system wrapper

#include "Ssd.h"

using namespace SSDSim;
using namespace std;

Ssd::Ssd(uint id, string dev, string sys, string pwd, string trc){
	uint i, j;
	
	controller= new Controller(this);
	packages= new vector<Package>();
	Die *die;

	for (i= 0; i < NUM_PACKAGES; i++){
		Package pack;
		pack.channel= new Channel();
		pack.channel->attachController(controller);
		for (j= 0; j < NUM_DIES; j++){
			die= new Die();
			cout<<pack.channel<<endl;
			die->attachToChannel(pack.channel);
			pack.dies.push_back(die);
		}
		packages->push_back(pack);
	}
	controller->attachPackages(packages);
	
	ReturnReadData= NULL;
	WriteDataDone= NULL;

	currentClockCycle= 0;
}

bool Ssd::add(Transaction &trans){
	controller->addTransaction(trans);
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
	uint i, j;
	Package package;

	for (i= 0; i < packages->size(); i++){
		package= (*packages)[i];
		for (j= 0; j < package.dies.size() ; j++){
			package.dies[j]->update();
			package.dies[j]->step();
		}
	}
	controller->update();
	controller->step();

	step();
}
