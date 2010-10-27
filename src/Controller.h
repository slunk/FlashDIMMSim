#ifndef CONTROLLER_H
#define CONTROLLER_H
//Controller.h
//header file for controller

#include "SimObj.h"
#include "FlashConfiguration.h"
#include "Die.h"
#include "Channel.h"
#include "FlashTransaction.h"

namespace FDSim{
	typedef struct {
		Channel *channel;
		std::vector<Die *> dies;
	} Package;

	class FlashDIMM;
	class Controller : public SimObj{
		public:
			Controller(FlashDIMM* parent);

			bool addTransaction(FlashTransaction &trans);
			void attachPackages(vector<Package> *packages);
			void returnReadData(const FlashTransaction &trans);
			void attachChannel(Channel *channel);
			void receiveFromChannel(ChannelPacket *busPacket);
			bool addPacket(ChannelPacket *p);
			void update(void);
			FlashDIMM *parentFlashDIMM;
			std::list<FlashTransaction> transactionQueue;
		private:

			std::list<FlashTransaction> returnTransaction;

			//Ftl ftl;
			std::vector<Package> *packages;
			std::vector<std::queue <ChannelPacket *> > channelQueues;
			std::vector<ChannelPacket *> outgoingPackets;
			
			std::vector<uint> channelXferCyclesLeft;

	};
}
#endif
