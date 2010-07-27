#ifndef PLANE_H
#define PLANE_H
//Plane.h
//header file for the Plane class

#include "FlashConfiguration.h"
#include "Block.h"
#include "ChannelPacket.h"

namespace FDSim{
	class Plane{
		public:
			Plane(void);
			void read(ChannelPacket *busPacket);
			void write(ChannelPacket *busPacket);
			void erase(ChannelPacket *busPacket);
			void storeInData(ChannelPacket *busPacket); 
			ChannelPacket *readFromData(void);
		private:
			ChannelPacket *dataReg, *cacheReg;
			std::map<uint, Block> blocks;
	};
}
#endif
