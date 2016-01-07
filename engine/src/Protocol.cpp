#include "Protocol.h"
#include "NetService.h"
#include "Packet.h"
#include "HandlerMgr.h"
#include "Log.h"

CU_NS_BEGIN

void PacketProtocol::process(Session* sess)
{
	// parse packet and post to process
	pt_decoder decoder(&sess->getReader());
	for (;;)
	{
		if (!decoder.parse())
			break;
	 	IHandler* handler = HandlerMgr::instance().find(decoder.msgid());
		if (handler)
		{
			pt_message* msg = handler->create();
			// decoder
			if (decoder.decode(*msg))
			{
				PacketEvent* ev = new PacketEvent();
				ev->sess = sess;
				ev->handler = handler;
				ev->msg = msg;
				gNetService->post(ev);
			}
			else
			{
				delete msg;
				// todo:dump msg
				LOG_ERROR("decode msg fail:msgid = %d, msglen = %d", decoder.msgid(), decoder.msglen());
			}
		}
		else
		{
			BufferEvent* ev = new BufferEvent();
			ev->sess = sess;
			ev->buff = decoder.split();
			gNetService->post(ev);
		}
		// for next
		decoder.discard();
	}
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
