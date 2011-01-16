#ifndef CHANNELPACKET_H
#define CHANNELPACKET_H
//ChannelPacket.h
//
//Header file for bus packet object
//

#include "FlashConfiguration.h"

namespace FDSim
{
	enum ChannelPacketType
	{
		READ,
		WRITE,
		ERASE,
		DATA
	};

	class ChannelPacket
	{
	public:
		//Fields
		ChannelPacketType busPacketType;
		uint page;
		uint block;
		uint plane;
		uint die;
		uint package;
		uint64_t virtualAddress;
		uint64_t physicalAddress;
		void *data;

		//Functions
		ChannelPacket(ChannelPacketType packtype, uint64_t virtualAddr, uint64_t physicalAddr, uint page, uint block, uint plane, uint die, uint package, void *dat);
		ChannelPacket();

		//void print();
		void print(uint64_t currentClockCycle);
		static void printData(const void *data);
	};
}

#endif

