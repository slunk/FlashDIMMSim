#include <iostream>
#include "SystemConfiguration.h"
#include "Ssd.h"
#include "Transaction.h"
#include <time.h>

#define SIM_CYCLES 30

/*temporary assignments for externed variables.
 * Using values from a samsung ssd*/
uint NUM_PACKAGES= 1;
uint NUM_DIES= 2;
uint NUM_PLANES= 4;
uint NUM_BLOCKS= 2048;
uint NUM_PAGES= 64;
uint PAGE_SIZE= 4;

uint READ_TIME= 25;
uint WRITE_TIME= 200;
uint ERASE_TIME= 1500;
uint DATA_TIME= 100;
uint COMMAND_TIME= 10;

uint SHOW_SIM_OUTPUT= 0;

using namespace SSDSim;
using namespace std;

int main(void){
	int cycle, numCycles= SIM_CYCLES;
	Ssd *ssd= new Ssd(0,"","","","");

	for (cycle= 0; cycle<numCycles; cycle++){
		(*ssd).update();
	}
	return 0;
}
