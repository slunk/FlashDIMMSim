//Block.cpp
//Block functions

#include "Block.h"

using namespace std;
using namespace SSDSim;

Block::Block(uint block){
	block_num= block;
}

Block::~Block(void){
	pages.~map();
}

void Block::read(uint page_num){
	if (pages.find(page_num) == pages.end()){
		cerr<<"Invalid read\n";
		exit(1);
	} else{
		//READ
	}
}

void Block::write(uint page_num, void *data){
	if (pages.find(page_num) == pages.end()){
		Page new_page;
		new_page.page_num= page_num;
		new_page.data= data;
		pages[page_num]= new_page;
	} else{
		cerr<<"Invalid write\n";
		exit(1);
	}
}

void Block::erase(){
	pages.clear();
}

