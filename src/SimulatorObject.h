#ifndef SIMULATOROBJ_H
#define SIMULATOROBJ_H

//SimulatorObject.h
//
//Header file for simulator object class
//
//Code copied from DRAMSim

#include <stdint.h>

namespace FDSim
{
	class SimulatorObject
	{
	public:
		uint64_t currentClockCycle;
		
		void step();
		virtual void update()=0;
	};
}

#endif

