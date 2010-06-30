//Ftl.cpp
//class file for ftl
//
#include "Ftl.h"
#include "BusPacket.h"

using namespace SSDSim;
using namespace std;

Ftl::Ftl(void){}

//dumb direct mapping for now
BusPacket *Ftl::translate(BusPacketType type, Transaction &trans){
	uint package, die, plane, block, page;
	package= trans.address / PACKAGE_SIZE;
	die= (trans.address % PACKAGE_SIZE) / DIE_SIZE;
	plane= (trans.address % PACKAGE_SIZE % DIE_SIZE) / PLANE_SIZE;
	block= (trans.address % PACKAGE_SIZE % DIE_SIZE % PLANE_SIZE) / BLOCK_SIZE;
	page= (trans.address % PACKAGE_SIZE % DIE_SIZE % PLANE_SIZE % BLOCK_SIZE) / PAGE_SIZE;
	//cout<<"package: "<<package<<" die: "<<die<<" plane: "<<plane<<" block: "<<block<<" page: "<<page<<endl;
	return new BusPacket(type, trans.address, page, block, plane, die, package, trans.data);
}

//garbage collection here eventually?
void Ftl::update(void){}
