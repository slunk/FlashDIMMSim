#ifndef CHANNEL_H
#define CHANNEL_H
//Channel.h
//header file for the Package class

#include "FlashConfiguration.h"
#include "Die.h"
#include "ChannelPacket.h"

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
			void sendToDie(uint die_num, ChannelPacket *busPacket);
			void sendToController(ChannelPacket *busPacket);
			Controller *controller;
		private:
			ChannelStatus status;
			uint sender;
			std::vector<Die *> dies;
	};
}
#endif
