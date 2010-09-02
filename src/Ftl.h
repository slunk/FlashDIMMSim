#ifndef FTL_H
#define FTL_H
//Ftl.h
//header file for the ftl

#include "SimObj.h"
#include "FlashConfiguration.h"
#include "ChannelPacket.h"
#include "FlashTransaction.h"

namespace FDSim{
	class Controller;
	class Ftl : public SimObj{
		public:
			Ftl(void);
			ChannelPacket *translate(ChannelPacketType type, FlashTransaction &trans);
			void update(void);
		private:
			uint offset, pageBitWidth, blockBitWidth, planeBitWidth, dieBitWidth, packageBitWidth;
			uint64_t writePointer;
			std::unordered_map<uint64_t,uint64_t> addressMap;
	};
}
#endif
