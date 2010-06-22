//Plane.h
//header file for the Plane class

#ifndef PLANE_H
#define PLANE_H

#include "SystemConfiguration.h"
#include "Block.h"
#include "BusPacket.h"

namespace SSDSim{
	class Plane{
		public:
			Plane(void);
			void read(BusPacket busPacket);
			void write(BusPacket busPacket);
			void erase(BusPacket busPacket);
			void storeInData(BusPacket busPacket); 
			BusPacket readFromData(void);
		private:
			BusPacket dataReg, cacheReg;
			std::map<uint, Block> blocks;
	};
}
#endif
