//! Server
#include "Cute/Protocal.h"
#include "Cute/Packet.h"
#include "Cute/Exception.h"
#include "Cute/BufferList.h"
#include "Cute/Server.h"
#include "Cute/HandlerService.h"
#include "Cute/BitMask.h"
#include "Cute/SessionEvent.h"

CUTE_NS_BEGIN

PacketProtocal::PacketProtocal(size_t maxSize /* = (size_t)(-1) */)
	: m_maxSize(maxSize)
{

}

void PacketProtocal::process(Session* sess, BufferList& buffer)
{
	bool result = true;
	for (;;)
	{
		// ��ȡhead
		BitMask8 bits;

		uint8 flag;
		if (!buffer.read(flag))
			break;

		bits = flag;

		if (bits.test(Packet::MASK_TEXT))
		{
			result = false;
			break;
		}

		// �ȶ�ȡ����
		uint32 length;
		uint32 msgid;
		if (!buffer.read7Bit(length))
			break;

		if (!buffer.read7Bit(msgid))
			break;

		if (length > m_maxSize)
		{
			result = false;
			break;
		}

		// ���Ȳ���,�ȴ���һ�ν���
		if (length < buffer.size())
		{
			break;
		}

		bool isTransfer = bits.test(Packet::MASK_TRANSFER);

		if (isTransfer)
		{
			//TransferEvent* ev = new TransferEvent(sess);
		}
		else
		{

		}

		EventHandler* handler = HandlerService::instance().find(msgid);
		if (handler == NULL || !handler->canAccept(sess->getType()))
		{// У���Ƿ��ܴ������Ϣ
			buffer.seek(length, SEEK_SET);
			buffer.discard();
			continue;
		}

		// ����ͷ��Ϣ
		uint32 status;
		uint32 gid;
		uint64 uid;
		if (bits.test(Packet::MASK_STATUS))
		{
			if (!buffer.read7Bit(status))
			{
				result = false;
				break;
			}
		}

		if (bits.test(Packet::MASK_GID))
		{
			if (!buffer.read7Bit(gid))
			{
				result = false;
				break;
			}
		}

		if (bits.test(Packet::MASK_UID))
		{
			if (!buffer.read7Bit(uid))
			{
				result = false;
				break;
			}
		}

		// ������Ϣ
		Packet* packet = handler->decode(buffer);
		if (packet == NULL)
		{// ����
			break;
		}

		//PacketEvent* pe = new PacketEvent(sess, packet);
		//Server::get().post(pe);
	}
}

DelimiterProtocal::DelimiterProtocal(size_t maxSize, char delimiter /* = '' */)
	: m_maxSize(maxSize)
	, m_delimiter(delimiter)
{
}

void DelimiterProtocal::process(Session* sess, BufferList& buffer)
{
	buffer.seek(0, SEEK_SET);

	for (;;)
	{
		size_t pos = buffer.find(m_delimiter);
		if (pos == NPOS)
		{// û���ҵ�
			if (buffer.size() > m_maxSize)
				throw RuntimeException("delimiter out of maxSize");

			// ����ѭ��
			break;
		}

		if (pos > m_maxSize)
			throw RuntimeException("delimiter out of maxSize");

		// ������Ϣ
		String text = buffer.toString(pos);
		TextEvent* ev = new TextEvent(sess, text);
		Server::get().post(ev);
	}
}

CUTE_NS_END
