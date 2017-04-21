#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

class BufferList;
class Session;
// 消息协议处理
class CUTE_CORE_API Protocal : public Ref<Protocal>
{
public:
	virtual ~Protocal(){}

	virtual void process(Session* sess, BufferList& buffer) = 0;
};

// 消息包
class CUTE_CORE_API PacketProtocal : public Protocal
{
public:
	PacketProtocal(size_t maxSize = (size_t)(-1));
	void process(Session* sess, BufferList& buffer);

private:
	size_t m_maxSize;
};

// 文本格式,\n结束
class CUTE_CORE_API DelimiterProtocal : public Protocal
{
public:
	DelimiterProtocal(size_t maxSize = 1024, char delimiter = '\n');

	void process(Session* sess, BufferList& buffer);

private:
	char m_delimiter;
	size_t m_maxSize;
};

CUTE_NS_END
