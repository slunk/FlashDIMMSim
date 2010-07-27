//Ftl.cpp
//class file for ftl
//
#include "Ftl.h"
#include "ChannelPacket.h"

using namespace FDSim;
using namespace std;

Ftl::Ftl(void){}

//dumb direct mapping for now
ChannelPacket *Ftl::translate(ChannelPacketType type, FlashTransaction &trans){
	uint package, die, plane, block, page;

	if (trans.address > (TOTAL_SIZE - PAGE_SIZE) || trans.address < 0 || (trans.address % PAGE_SIZE) != 0){
		ERROR("Inavlid address in Ftl");
		exit(1);
	}

	package= trans.address / PACKAGE_SIZE;
	die= (trans.address % PACKAGE_SIZE) / DIE_SIZE;
	plane= (trans.address % PACKAGE_SIZE % DIE_SIZE) / PLANE_SIZE;
	block= (trans.address % PACKAGE_SIZE % DIE_SIZE % PLANE_SIZE) / BLOCK_SIZE;
	page= (trans.address % PACKAGE_SIZE % DIE_SIZE % PLANE_SIZE % BLOCK_SIZE) / PAGE_SIZE;
	//cout<<package<<" "<<die<<" "<<plane<<" "<<block<<" "<<" "<<page<<endl;

	return new ChannelPacket(type, trans.address, page, block, plane, die, package, trans.data);
}

//garbage collection here eventually?
void Ftl::update(void){}
