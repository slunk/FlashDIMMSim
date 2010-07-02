//FlashDIMM.cpp
//Class file for flash fimm system wrapper

#include "FlashDIMM.h"
//#include "IniReader.h"

using namespace FDSim;
using namespace std;

FlashDIMM::FlashDIMM(uint id, string dev, string sys, string pwd, string trc){
	uint i, j;
	
	controller= new Controller(this);
	packages= new vector<Package>();
	Die *die;

	for (i= 0; i < NUM_PACKAGES; i++){
		Package pack;
		pack.channel= new Channel();
		pack.channel->attachController(controller);
		for (j= 0; j < DIES_PER_PACKAGE; j++){
			die= new Die(this);
			die->attachToChannel(pack.channel);
			pack.dies.push_back(die);
		}
		packages->push_back(pack);
	}
	controller->attachPackages(packages);
	
	//get device parameters
	//IniReader::ReadIniFile(dev, false);

	ReturnReadData= NULL;
	WriteDataDone= NULL;

	numReads= 0;
	numWrites= 0;
	numErases= 0;
	currentClockCycle= 0;
}

bool FlashDIMM::add(Transaction &trans){
	controller->addTransaction(trans);
	return true;
}

bool FlashDIMM::addTransaction(bool isWrite, uint64_t addr){
	return true;
}

string FlashDIMM::SetOutputFileName(string tracefilename){
	return "";
}

void FlashDIMM::RegisterCallbacks(Callback_t *readCB, Callback_t *writeCB){
	ReturnReadData = readCB;
	WriteDataDone = writeCB;
}

void FlashDIMM::printStats(void){
}

void FlashDIMM::update(void){
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
