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
extern uint NUM_DIES;
extern uint NUM_PLANES;
extern uint NUM_BLOCKS;
extern uint NUM_PAGES;
extern uint PAGE_SIZE;

#define BLOCK_SIZE (PAGE_SIZE*NUM_PAGES)
#define PLANE_SIZE (PAGE_SIZE*NUM_PAGES*NUM_BLOCKS)
#define DIE_SIZE (PAGE_SIZE*NUM_PAGES*NUM_BLOCKS*NUM_PLANES)
#define PACKAGE_SIZE (PAGE_SIZE*NUM_PAGES*NUM_BLOCKS*NUM_PLANES*NUM_DIES)
#define TOTAL_SIZE (PAGE_SIZE*NUM_PAGES*NUM_BLOCKS*NUM_PLANES*NUM_DIES*NUM_PACKAGES)

extern uint READ_TIME;
extern uint WRITE_TIME;
extern uint ERASE_TIME;
extern uint DATA_TIME;
extern uint COMMAND_TIME;

extern uint SHOW_SIM_OUTPUT;

namespace SSDSim{
	typedef void (*returnCallBack_t)(uint id, uint64_t addr, uint64_t clockcycle);
}
#endif
