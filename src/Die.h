//Die.h
//header file for the Die class

#ifndef DIE_H
#define DIE_H



namespace SSDSim{

	class Ssd;
	class Die : public SimulatorObject{
		public:
			void receive_from_bus(BusPacket &packet);
			void update(void);

			//fields
			vector<Plane> planes;
		private:
			Channel channel;
			uint control_cycles_left;
			uint data_cycles_left;
	};
}
#endif
