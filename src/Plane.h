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
			void read(BusPacket command_packet);
			void write(BusPacket command_packet);
			void erase(BusPacket command_packet);
			void store_in_data(BusPacket data_packet); 
			void store_in_cache(BusPacket data_packet); 
			void move_data_to_cache(void);
			void move_cache_to_data(void);
		private:
			BusPacket data_reg, cache_reg;
			map<uint, Block> blocks;
	};
}
#endif
