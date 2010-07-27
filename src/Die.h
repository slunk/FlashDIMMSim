#ifndef DIE_H
#define DIE_H
//Die.h
//header file for the Die class

#include "SimObj.h"
#include "FlashConfiguration.h"
#include "ChannelPacket.h"
#include "Plane.h"

namespace FDSim{

	class Channel;
	class Controller;
	class FlashDIMM;
	class Die : public SimObj{
		public:
			Die(FlashDIMM *parent);
			void attachToChannel(Channel *chan);
			void receiveFromChannel(ChannelPacket *busPacket);
			int isPlaneBusy(ChannelPacket *busPacket);
			void update(void);
		private:
			FlashDIMM *parentFlashDIMM;
			Channel *channel;
			uint dataCyclesLeft;
			//std::vector<uint> controlCyclesLeft;//since separate planes can theoretically read, write, and erase at the same time
			std::queue<ChannelPacket *> returnDataPackets;
			std::vector<Plane> planes;
			ChannelPacket *currentCommand;

			//for first implementation without contention
			std::queue<ChannelPacket *> commands;
			uint controlCyclesLeft;
	};
}
#endif
