//Block.cpp
//Block functions

#include "Block.h"

using namespace std;
using namespace SSDSim;

Block::Block(uint block){
	block_num= block;
}

Block::Block(){}

void *Block::read(uint page_num){
	if (page_data.find(page_num) == page_data.end()){
		ERROR("Request to read page "<<page_num<<" failed: nothing has been written to that address");
		exit(1);
	} else{
		//READ
		//cout<<page_data[page_num]<<endl;
		return page_data[page_num];
	}
}

void Block::write(uint page_num, void *data){
	if (page_data.find(page_num) == page_data.end()){
		page_data[page_num]= data;
	} else{
		/*ERROR("Request to write page "<<page_num<<" failed: page has been written to and not erased"); 
		exit(1);*/
		
		//Until garbage collection is implemented, writes dirty/valid but written to pages is allowed
		page_data[page_num]= data;
	}
}

void Block::erase(){
	page_data.clear();
}

