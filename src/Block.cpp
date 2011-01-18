//Block.cpp
//Block functions

#include "Block.h"

using namespace std;
using namespace FDSim;

Block::Block(uint block){
	block_num= block;
}

Block::Block(){}

void *Block::read(uint page_num){
	if (page_data.find(page_num) == page_data.end()){
		DEBUG("Request to read page "<<page_num<<" failed: nothing has been written to that address");
		return (void *)0x0;
	} else{
		return page_data[page_num];
	}
}

void Block::write(uint page_num, void *data){
	if (page_data.find(page_num) == page_data.end()){
		page_data[page_num]= data;
	} else{
		ERROR("Request to write page "<<page_num<<" failed: page has been written to and not erased"); 
		exit(1);
	}
}

void Block::erase(){
	page_data.clear();
}

