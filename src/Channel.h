#ifndef CHANNEL_H
#define CHANNEL_H
//Channel.h
//header file for the Package class

#include "FlashConfiguration.h"
#include "Die.h"
#include "ChannelPacket.h"

namespace FDSim{
	enum SenderType{
		CONTROLLER,
		DIE
	};

	class Controller;
	class Channel{
		public:
			Channel(void);
			void attachDie(Die *d);
			void attachController(Controller *c);
			int obtainChannel(uint s, SenderType t, ChannelPacket *p);
			int releaseChannel(SenderType t, uint s);
			int hasChannel(SenderType, uint s);
			void sendToDie(ChannelPacket *busPacket);
			void sendToController(ChannelPacket *busPacket);
			Controller *controller;
		private:
			SenderType type;
			int sender;
			std::vector<Die *> dies;
	};
}
#endif
