#ifndef FTL_H
#define FTL_H
//Ftl.h
//header file for the ftl

#include "SimulatorObject.h"
#include "SystemConfiguration.h"
#include "BusPacket.h"
#include "Transaction.h"

namespace SSDSim{
	class Controller;
	class Ftl : public SimulatorObject{
		public:
			Ftl(void);
			BusPacket *translate(BusPacketType type, Transaction &trans);
			void update(void);
		private:
			uint64_t writePointer;
			std::map<uint64_t,uint64_t> addressMap;
	};
}
#endif
