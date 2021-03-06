#ifndef FLASHCONF_H
#define FLASHCONF_H
//SysemConfiguration.h
//Configuration values, headers, and macros for the whole system
//
#include <iostream>
#include <cstdlib>
#include <string>

#include <vector>
#include <unordered_map>
#include <queue>
#include <list>
#include <stdint.h>
#include <limits.h>

//sloppily reusing #defines from dramsim
#ifndef ERROR
#define ERROR(str) std::cerr<<"[ERROR ("<<__FILE__<<":"<<__LINE__<<")]: "<<str<<std::endl;
#endif

#ifdef DEBUG_BUILD
	#ifndef DEBUG
	#define DEBUG(str) std::cout<< str <<std::endl;
	#endif
	#ifndef DEBUGN
	#define DEBUGN(str) std::cout<< str;
	#endif
#else
	#ifndef DEBUG
	#define DEBUG(str) ;
	#endif
	#ifndef DEBUGN
	#define DEBUGN(str) ;
	#endif
#endif

#ifndef NO_OUTPUT
	#ifndef PRINT
	#define PRINT(str)  if(OUTPUT) { std::cerr <<str<<std::endl; }
	#endif
	#ifndef PRINTN
	#define PRINTN(str) if(OUTPUT) { std::cerr <<str; }
	#endif
#else
	#undef DEBUG
	#undef DEBUGN
	#define DEBUG(str) ;
	#define DEBUGN(str) ;
	#ifndef PRINT
	#define PRINT(str) ;
	#endif
	#ifndef PRINTN
	#define PRINTN(str) ;
	#endif
#endif

extern uint NUM_PACKAGES;
extern uint DIES_PER_PACKAGE;
extern uint PLANES_PER_DIE;
extern uint BLOCKS_PER_PLANE;
extern uint VIRTUAL_BLOCKS_PER_PLANE;
extern uint PAGES_PER_BLOCK;
extern uint FLASH_PAGE_SIZE;

extern float IDLE_GC_THRESHOLD;
extern float FORCE_GC_THRESHOLD;
extern float PBLOCKS_PER_VBLOCK;

#define BLOCK_SIZE (FLASH_PAGE_SIZE * PAGES_PER_BLOCK)
#define PLANE_SIZE (FLASH_PAGE_SIZE * BLOCKS_PER_PLANE * PAGES_PER_BLOCK)
#define DIE_SIZE (FLASH_PAGE_SIZE * PLANES_PER_DIE * BLOCKS_PER_PLANE * PAGES_PER_BLOCK)
#define PACKAGE_SIZE (FLASH_PAGE_SIZE * DIES_PER_PACKAGE * PLANES_PER_DIE * BLOCKS_PER_PLANE * PAGES_PER_BLOCK)
#define TOTAL_SIZE (FLASH_PAGE_SIZE * NUM_PACKAGES * DIES_PER_PACKAGE * PLANES_PER_DIE * BLOCKS_PER_PLANE * PAGES_PER_BLOCK)

#define VIRTUAL_PLANE_SIZE (FLASH_PAGE_SIZE * VIRTUAL_BLOCKS_PER_PLANE * PAGES_PER_BLOCK)
#define VIRTUAL_DIE_SIZE (FLASH_PAGE_SIZE * PLANES_PER_DIE * VIRTUAL_BLOCKS_PER_PLANE * PAGES_PER_BLOCK)
#define VIRTUAL_PACKAGE_SIZE (FLASH_PAGE_SIZE * DIES_PER_PACKAGE * PLANES_PER_DIE * VIRTUAL_BLOCKS_PER_PLANE PAGES_PER_BLOCK)
#define VIRTUAL_TOTAL_SIZE (FLASH_PAGE_SIZE * NUM_PACKAGES * DIES_PER_PACKAGE * PLANES_PER_DIE * VIRTUAL_BLOCKS_PER_PLANE * PAGES_PER_BLOCK)


extern uint READ_TIME;
extern uint WRITE_TIME;
extern uint ERASE_TIME;
extern uint DATA_TIME;
extern uint COMMAND_TIME;
extern uint LOOKUP_TIME;

extern uint OUTPUT;

namespace FDSim{
	typedef void (*returnCallBack_t)(uint id, uint64_t addr, uint64_t clockcycle);
}
#endif
