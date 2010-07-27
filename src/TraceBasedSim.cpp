/*TraceBasedSim.cpp
 *
 * This will eventually run traces. Right now the name is a little misleading...
 * It adds a certain amount (NUM_WRITES) of write transactions to the flash dimm
 * linearly starting at address 0 and then simulates a certain number (SIM_CYCLES)
 * of cycles before exiting.
 *
 * The output should be fairly straightforward. If you would like to see the writes
 * as they take place, change OUTPUT= 0; to OUTPUT= 1;
 */
#include <iostream>
#include "FlashConfiguration.h"
#include "FlashDIMM.h"
#include "FlashTransaction.h"
#include <time.h>

#define NUM_WRITES 100
#define SIM_CYCLES 100000

/*temporary assignments for externed variables.
 * This should really be done with another class
 * that reads .ini files
 *
 * values from a samsung flash dimm:
 * */

/*uint NUM_PACKAGES= 1;
uint DIES_PER_PACKAGE= 2;
uint PLANES_PER_DIE= 4;
uint BLOCKS_PER_PLANE= 2048;
uint PAGES_PER_BLOCK= 64;
uint PAGE_SIZE= 4;

uint READ_TIME= 25;
uint WRITE_TIME= 200;
uint ERASE_TIME= 1500;
uint DATA_TIME= 100;
uint COMMAND_TIME= 10;
*/
uint OUTPUT= 1;

using namespace FDSim;
using namespace std;

int main(void){
	clock_t start= clock(), end;
	uint write, cycle;
	FlashDIMM *flashDimm= new FlashDIMM(0,"ini/samsung_K9XXG08UXM.ini","ini/def_system.ini","","");
	FlashTransaction t;

	for (write= 0; write<NUM_WRITES; write++){
		t= FlashTransaction(DATA_WRITE, write*4, (void *)0xdeadbeef);
		(*flashDimm).add(t);
	}

	for (cycle= 0; cycle<SIM_CYCLES; cycle++){
		(*flashDimm).update();
		/*if (cycle < NUM_WRITES){
			t= FlashTransaction(DATA_READ, cycle*4, (void *)0xfeedface);
			(*flashDimm).add(t);
		}*/
		if (flashDimm->numWrites == NUM_WRITES)
			break;
	}

	end= clock();
	cout<<"Simulation Results:\n";
	cout<<"Cycles simulated: "<<cycle<<endl;
	cout<<"Reads completed: "<<flashDimm->numReads<<endl;
	cout<<"Writes completed: "<<flashDimm->numWrites<<endl;
	cout<<"Erases completed: "<<flashDimm->numErases<<endl;
	cout<<"Execution time: "<<(end-start)<<" cycles. "<<(double)(end-start)/CLOCKS_PER_SEC<<" seconds.\n";

	return 0;
}
