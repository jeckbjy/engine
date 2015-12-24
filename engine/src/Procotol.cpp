#include "Procotol.h"
#include "NetService.h"
#include "Proto.h"

CU_NS_BEGIN

void PacketProtocol::process(Session* sess)
{
	// parse packet and post to process
	PacketEvent* ev = new PacketEvent();
	ev->sess = sess;
	ev->msg = 0;
	gNetService->post(ev);

}

CU_NS_END