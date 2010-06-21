//Block.cpp
//Block functions

#include "Block.h"

using namespace std;
using namespace SSDSim;

Block::Block(uint block){
	block_num= block;
}

void Block::read(uint page_num){
	if (page_data.find(page_num) == page_data.end()){
		cerr<<"Invalid read\n";
		exit(1);
	} else{
		//READ
		cout<<page_data[page_num]<<endl;
	}
}

void Block::write(uint page_num, void *data){
	if (page_data.find(page_num) == page_data.end()){
		page_data[page_num]= data;
	} else{
		cerr<<"Invalid write\n";
		exit(1);
	}
}

void Block::erase(){
	page_data.clear();
}

