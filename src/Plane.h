//Plane.h
//header file for the Plane class

#ifndef PLANE_H
#define PLANE_H



namespace SSDSim{
	class Plane{
		public:
			//functions
			void read(uint block_num, uint page_num);
			void write(uint block_num, uint page_num);
			void erase(uint block_num);
			//fields
			void *data_reg, *cache_reg;
		private:
			map<uint, Block> blocks;
	};
}
#endif
