//Die.h
//header file for the Die class

#ifndef DIE_H
#define DIE_H

#include "SimulatorObject.h"
#include "SystemConfiguration.h"
#include "BusPacket.h"
#include "Plane.h"

namespace SSDSim{

	class Channel;
	class Die : public SimulatorObject{
		public:
			Die(void);
			void attachToChannel(Channel *chan);
			void receiveFromChannel(BusPacket *busPacket);
			void update(void);
		private:
			Channel *channel;
			uint dataCyclesLeft;
			//std::vector<uint> controlCyclesLeft;//since separate planes can theoretically read, write, and erase at the same time
			std::queue<BusPacket *> returnDataPackets;
			std::vector<Plane> planes;
			BusPacket *currentCommand;

			//for first implementation without contention
			std::queue<BusPacket *> commands;
			uint controlCyclesLeft;
	};
}
#endif
