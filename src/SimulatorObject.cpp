//SimulatorObject.cpp
//
//Base class for all classes in the simulator
//	
//Code copied from DRAMSim

#include <cstdlib>
#include "SimulatorObject.h"

using namespace FDSim;
using namespace std;

void SimulatorObject::step()
{
	currentClockCycle++;
}


