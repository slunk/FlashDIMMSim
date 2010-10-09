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
			Die(FlashDIMM *parent, uint id);
			void attachToChannel(Channel *chan);
			void receiveFromChannel(ChannelPacket *busPacket);
			int isDieBusy(uint plane);
			void update(void);
		private:
			uint id;
			FlashDIMM *parentFlashDIMM;
			Channel *channel;
			uint dataCyclesLeft;
			std::queue<ChannelPacket *> returnDataPackets;
			std::vector<Plane> planes;
			//ChannelPacket *currentCommand;
			std::vector<ChannelPacket *> currentCommands;
			std::vector<uint> controlCyclesLeft;

			//for first implementation without contention
			//std::queue<ChannelPacket *> commands;
			//uint controlCyclesLeft;
	};
}
#endif
