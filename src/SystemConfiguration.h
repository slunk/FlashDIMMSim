#ifndef SYSCONFIG_H
#define SYSCONFIG_H
//SysemConfiguration.h
//Configuration values, headers, and macros for the whole system
//
#include <iostream>
#include <cstdlib>
#include <string>

#include <vector>
#include <map>
#include <queue>

#include <stdint.h>

#define ERROR(str) std::cerr<<"[ERROR ("<<__FILE__<<":"<<__LINE__<<")]: "<<str<<std::endl;

#ifdef DEBUG_BUILD
	#define DEBUG(str) std::cout<< str <<std::endl;
	#define DEBUGN(str) std::cout<< str;
#else
	#define DEBUG(str) ;
	#define DEBUGN(str) ;
#endif

#ifndef NO_OUTPUT
	#define PRINT(str)  if(SHOW_SIM_OUTPUT) { std::cerr <<str<<std::endl; }
	#define PRINTN(str) if(SHOW_SIM_OUTPUT) { std::cerr <<str; }
#else
	#undef DEBUG
	#undef DEBUGN
	#define DEBUG(str) ;
	#define DEBUGN(str) ;
	#define PRINT(str) ;
	#define PRINTN(str) ;
#endif

extern uint NUM_PACKAGES;
extern uint DIES_PER_PACKAGE;
extern uint PLANES_PER_DIE;
extern uint BLOCKS_PER_PLANE;
extern uint PAGES_PER_BLOCK;
extern uint PAGE_SIZE;

#define BLOCK_SIZE (PAGE_SIZE * PAGES_PER_BLOCK)
#define PLANE_SIZE (PAGE_SIZE * BLOCKS_PER_PLANE * PAGES_PER_BLOCK)
#define DIE_SIZE (PAGE_SIZE * PLANES_PER_DIE * BLOCKS_PER_PLANE * PAGES_PER_BLOCK)
#define PACKAGE_SIZE (PAGE_SIZE * DIES_PER_PACKAGE * PLANES_PER_DIE * BLOCKS_PER_PLANE * PAGES_PER_BLOCK)
#define TOTAL_SIZE (PAGE_SIZE * NUM_PACKAGES * DIES_PER_PACKAGE * PLANES_PER_DIE * BLOCKS_PER_PLANE * PAGES_PER_BLOCK)

extern uint READ_TIME;
extern uint WRITE_TIME;
extern uint ERASE_TIME;
extern uint DATA_TIME;
extern uint COMMAND_TIME;

extern uint SHOW_SIM_OUTPUT;

namespace FDSim{
	typedef void (*returnCallBack_t)(uint id, uint64_t addr, uint64_t clockcycle);
}
#endif
