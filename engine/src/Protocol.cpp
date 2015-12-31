#include "Protocol.h"
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

void TextProtocol::process(Session* sess)
{
	// 查找结束
	Buffer& reader = sess->getReader();
	uint rpos = reader.find('\r', 0);
	if (rpos == Buffer::npos)
		return;
	uint epos = reader.find('\n', rpos);
	if (epos != rpos + 1)
		return;
	// 解析出value
	TextEvent* ev = new TextEvent();
	ev->sess = sess;
	reader.front(ev->text, rpos);
	reader.seek(epos + 1, SEEK_SET);
	reader.discard();
	gNetService->post(ev);
}

CU_NS_END
