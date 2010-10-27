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
	class Ftl : public SimObj{
		public:
			Ftl(Controller *c);
			ChannelPacket *translate(ChannelPacketType type, uint64_t addr);
			bool addTransaction(FlashTransaction &t);
			void update(void);
			Controller *controller;
		private:
			uint offset, pageBitWidth, blockBitWidth, planeBitWidth, dieBitWidth, packageBitWidth;
			uint channel, die, plane, lookupCounter;
			FlashTransaction currentTransaction;
			uint busy;
			std::unordered_map<uint64_t,uint64_t> addressMap;
			std::vector<vector<bool>> dirty;
			std::vector<vector<bool>> used;
			std::list<FlashTransaction> transactionQueue;
	};
}
#endif
