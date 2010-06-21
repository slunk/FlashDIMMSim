#ifndef BUSPACKET_H
#define BUSPACKET_H
//BusPacket.h
//
//Header file for bus packet object
//

#include "SystemConfiguration.h"

namespace SSDSim
{
	enum BusPacketType
	{
		READ,
		WRITE,
		ERASE,
		DATA
	};

	class BusPacket
	{
	public:
		//Fields
		BusPacketType busPacketType;
		uint page;
		uint block;
		uint plane;
		uint die;
		uint64_t physicalAddress;
		void *data;

		//Functions
		BusPacket(BusPacketType packtype, uint64_t physicalAddr, uint page, uint block, uint plane, uint die, void *dat);
		BusPacket();

		void print();
		void print(uint64_t currentClockCycle, bool dataStart);
		static void printData(const void *data);
	};
}

#endif

