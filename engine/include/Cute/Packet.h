#pragma once
#include "Cute/Foundation.h"
#include "Cute/Message.h"
#include "Cute/Ref.h"
#include "Cute/BufferList.h"

CUTE_NS_BEGIN

// 消息包，包括通用消息头和消息体
class CUTE_CORE_API Packet : public Message
{
public:
	enum Mask
	{
		MASK_TEXT		= 0x80,
		MASK_TRANSFER	= 0x40,
		MASK_STATUS		= 0x20,
		MASK_GID		= 0x08,
		MASK_UID		= 0x04,
	};

	Packet();

	bool hasFlag(Mask mask) const { return (m_flag & mask) == mask; }
	bool isText() const { return hasFlag(MASK_TEXT); }
	bool isTransfer() const { return hasFlag(MASK_TRANSFER); }

	void setMask(Mask mask) { m_flag |= mask; }
	void setStatus(uint32 status);
	void setGID(uint32 gid);
	void setUID(uint64 uid);

	void reset();

protected:
	uint8  m_flag;
	uint32 m_status;
	uint32 m_gid;
	uint64 m_uid;
};

template<typename TMSGID>
class TPacket : public Packet
{
public:
	enum { MSG_ID = TMSGID };
	size_t msgid() const { return MSG_ID; }
};

// 中转消息
struct TransferPacket : public Packet
{
	uint32 m_msgid;
	BufferList msg;
};

// 读取消息头
class PacketReader
{
public:
	PacketReader();
	~PacketReader();

protected:
	BufferList	m_buffer;
	uint8		m_flag;
};

class PacketWriter
{
public:
	PacketWriter();
	~PacketWriter();

protected:
	BufferList* m_buffer;
	Packet*		m_msg;
};

CUTE_NS_END
