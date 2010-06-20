//Block.h
//header file for the Block class

#ifndef BLOCK_H
#define BLOCK_H

#include "SystemConfiguration.h"

namespace SSDSim{
	typedef struct page{
		uint page_num;
		void *data;
	} Page;

	class Block{
		public:
			Block(uint block_num);
			~Block(void);
			void read(uint page_num);
			void write(uint page_num, void *data);
			void erase(void);
		private:
			uint block_num;
			std::map<uint, Page> pages;
	};
}
#endif
