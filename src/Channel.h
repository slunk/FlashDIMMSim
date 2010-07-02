#ifndef CHANNEL_H
#define CHANNEL_H
//Channel.h
//header file for the Package class

#include "SystemConfiguration.h"
#include "Die.h"
#include "BusPacket.h"

namespace FDSim{
	typedef enum{
		CHANNEL_FREE,
		CHANNEL_BUSY
	} ChannelStatus;
	
	class Controller;
	class Channel{
		public:
			Channel(void);
			void attachDie(Die *d);
			void attachController(Controller *c);
			int obtainChannel(uint sender);
			int releaseChannel(uint sender);
			int hasChannel(uint sender);
			void sendToDie(uint die_num, BusPacket *busPacket);
			void sendToController(BusPacket *busPacket);
			Controller *controller;
		private:
			ChannelStatus status;
			uint sender;
			std::vector<Die *> dies;
	};
}
#endif
