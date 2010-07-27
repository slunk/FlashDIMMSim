#ifndef FLASH_DIMM_H
#define FLASH_DIMM_H
//FlashDIMM.h
//Header for flash flash dimm system wrapper

#include "SimObj.h"
#include "FlashConfiguration.h"
#include "Controller.h"
#include "Die.h"
#include "FlashTransaction.h"
#include "Callbacks.h"

namespace FDSim{
	typedef CallbackBase<void,uint,uint64_t,uint64_t> Callback_t;
	class FlashDIMM : public SimObj{
		public:
			FlashDIMM(uint id, string dev, string sys, string pwd, string trc);
			void update(void);
			bool add(FlashTransaction &trans);
			bool addTransaction(bool isWrite, uint64_t addr);
			void printStats(void);
			string SetOutputFileName(string tracefilename);
			void RegisterCallbacks(Callback_t *readDone, Callback_t *writeDone);

			Controller *controller;
			vector<Package> *packages;

			//std::ofsream visDataOut;
			//
			Callback_t* ReturnReadData;
			Callback_t* WriteDataDone;

			uint systemID, numReads, numWrites, numErases;
			Package p;
		private:
	};
}
#endif
