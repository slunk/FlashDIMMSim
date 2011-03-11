#ifndef FTL_H
#define FTL_H
//Ftl.h
//header file for the ftl

#include "SimObj.h"
#include "FlashConfiguration.h"
#include "ChannelPacket.h"
#include "FlashTransaction.h"
#include "Controller.h"

namespace FDSim{
	class FlashDIMM;
	class Ftl : public SimObj{
		public:
			Ftl(FlashDIMM *p, Controller *c);
			ChannelPacket *translate(ChannelPacketType type, uint64_t vAddr, uint64_t pAddr);
			bool addTransaction(FlashTransaction &t);
			void update(void);
			bool checkGC(void); 
			void runGC(void); 
			uint64_t get_ptr(void); 
			void inc_ptr(void); 
			Controller *controller;
			FlashDIMM *parent;
		private:
			uint offset, pageBitWidth, blockBitWidth, planeBitWidth, dieBitWidth, packageBitWidth;
			uint channel, die, plane, lookupCounter, gc_status, panic_mode;
			uint64_t used_page_count, start_erase,  queue_size;
			FlashTransaction currentTransaction;
			uint busy;
			std::unordered_map<uint64_t,uint64_t> addressMap;
			std::vector<vector<bool>> dirty;
			std::vector<vector<bool>> used;
			std::list<FlashTransaction> transactionQueue;

			void addGcTransaction(FlashTransaction &t);
	};
}
#endif
