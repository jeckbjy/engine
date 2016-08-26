#include "Protocol.h"
#include "NetService.h"
#include "Packet.h"
#include "HandlerMgr.h"
#include "Log.h"

CU_NS_BEGIN

void PacketProtocol::process(Session* sess)
{
	// parse packet and post to process
	Buffer& buffer = sess->getReader();
	pt_decoder decoder(&buffer);
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
			// for next
			buffer.discard();
		}
		else
		{
			BufferEvent* ev = new BufferEvent();
			ev->sess = sess;
			buffer.split(ev->buff);
			gNetService->post(ev);
		}
	}
}

void TextProtocol::process(Session* sess)
{
	// 查找结束
	Buffer& reader = sess->getReader();
	uint rpos = reader.find("\r\n", 0);
	if (rpos == NPOS)
		return;

	reader.seek(rpos, SEEK_SET);
	// 解析出value
	TextEvent* ev = new TextEvent();
	ev->sess = sess;
	reader.split(ev->text);
	gNetService->post(ev);
}

CU_NS_END
