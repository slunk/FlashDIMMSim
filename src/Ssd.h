#ifndef SSD_H
#define SSD_H
//Sdd.h
//Header for flash ssd system wrapper

#include "SimulatorObject.h"
#include "SystemConfiguration.h"
#include "Controller.h"
#include "Die.h"
#include "Transaction.h"
#include "Callback.h"

namespace SSDSim{
	typedef CallbackBase<void,uint,uint64_t,uint64_t> Callback_t;
	class Ssd : public SimulatorObject{
		public:
			Ssd(uint id, string dev, string sys, string pwd, string trc);
			void update(void);
			bool add(Transaction &trans);
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

			uint systemID;
		private:
	};
}
#endif
