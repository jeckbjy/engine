//! Server
#include "Cute/Encoder.h"

CUTE_NS_BEGIN

Encoder::Encoder(Stream* stream)
	: m_stream(stream)
	, m_tag(0)
	, m_off(0)
{
}

Encoder::~Encoder()
{
}

size_t Encoder::encode(Message* msg)
{
	m_spos = m_stream->position();
	msg->encode(*this);
	return m_stream->position() - m_spos;
}

void Encoder::skip(size_t offset)
{
	m_tag += offset;
	m_off += offset;
}

void Encoder::write7Bit(uint64 data)
{
	char	buffer[10];
	size_t	length = encode7Bit(buffer, data);
	writeData(buffer, length);
}

void Encoder::writeData(const char* data, size_t length)
{
	m_stream->write(data, length);
}

void Encoder::writeTag(size_t tagOff, uint64 value, bool extra)
{
	char	buffer[20];
	size_t	length = encodeTag(buffer, tagOff, value, extra);

	// 写入数据
	writeData(buffer, length);
}

void Encoder::writeBeg(size_t& spos)
{
	spos = m_stream->position();
	m_stream->write(NULL, 10);
}

void Encoder::writeEnd(size_t spos)
{
	size_t  tagOff = m_off;

	size_t	epos = m_stream->position();
	size_t	size = epos - spos - 10;
	size_t	head_size;
	char	head_data[10];
	head_size = encodeTag(head_data, tagOff, size, true);

	m_stream->seek(spos, SEEK_SET);
	m_stream->write(head_data, head_size);
	size_t left_size = 10 - head_size;
	m_stream->erase(left_size);
	m_stream->seek(0, SEEK_END);
}

bool Encoder::writeField(Message& msg)
{
	size_t spos;
	writeBeg(spos);

	Encoder coder(m_stream);
	size_t size = coder.encode(&msg);

	writeEnd(spos);
	return true;
}

bool Encoder::writeField(const String& str)
{
	if (str.empty())
		return false;

	size_t length = str.size();
	writeTag(m_off, length, true);
	writeData(str.data(), length);
	return true;
}

bool Encoder::writeField(int8 data)
{
	if (data != 0)
	{
		writeTag(m_off, encodei64(data), false);
		return true;
	}

	return false;
}

bool Encoder::writeField(int16 data)
{
	if (data != 0)
	{
		writeTag(m_off, encodei64(data), false);
		return true;
	}

	return false;
}

bool Encoder::writeField(int32 data)
{
	if (data != 0)
	{
		writeTag(m_off, encodei64(data), false);
		return true;
	}

	return false;
}

bool Encoder::writeField(int64 data)
{
	if (data != 0)
	{
		writeTag(m_off, encodei64(data), false);
		return true;
	}

	return false;
}

bool Encoder::writeField(uint8 data)
{
	if (data != 0)
	{
		writeTag(m_off, data, false);
		return true;
	}

	return false;
}

bool Encoder::writeField(uint16 data)
{
	if (data != 0)
	{
		writeTag(m_off, data, false);
		return true;
	}

	return false;
}

bool Encoder::writeField(uint32 data)
{
	if (data != 0)
	{
		writeTag(m_off, data, false);
		return true;
	}

	return false;
}

bool Encoder::writeField(uint64 data)
{
	if (data != 0)
	{
		writeTag(m_off, data, false);
		return true;
	}

	return false;
}

bool Encoder::writeField(float data)
{
	if (data != 0)
	{
		writeTag(m_off, encodef32(data), false);
		return true;
	}

	return false;
}

bool Encoder::writeField(double data)
{
	if (data != 0)
	{
		writeTag(m_off, encodef64(data), false);
		return true;
	}

	return false;
}

void Encoder::writeItem(Message& msg)
{
	const size_t cHeadMax = 5;
	size_t spos = m_stream->position();
	// 空出空间
	m_stream->write(NULL, cHeadMax);
	Encoder coder(m_stream);
	size_t msg_size = coder.encode(&msg);
	char   head_data[cHeadMax];
	size_t head_size = encode7Bit(head_data, msg_size);
	size_t left_size = cHeadMax - head_size;
	m_stream->seek(spos, SEEK_SET);
	m_stream->write(head_data, head_size);
	m_stream->erase(left_size);		// 删除多余数据
	m_stream->seek(0, SEEK_END);
}

void Encoder::writeItem(const String& str)
{
	size_t size = str.size();
	write7Bit(size);
	if (size > 0)
		writeData(str.c_str(), str.size());
}

void Encoder::writeItem(int8 data)
{
	write7Bit(encodei64(data));
}

void Encoder::writeItem(int16 data)
{
	write7Bit(encodei64(data));
}

void Encoder::writeItem(int32 data)
{
	write7Bit(encodei64(data));
}

void Encoder::writeItem(int64 data)
{
	write7Bit(encodei64(data));
}

void Encoder::writeItem(uint8 data)
{
	write7Bit(data);
}

void Encoder::writeItem(uint16 data)
{
	write7Bit(data);
}

void Encoder::writeItem(uint32 data)
{
	write7Bit(data);
}

void Encoder::writeItem(uint64 data)
{
	write7Bit(data);
}

void Encoder::writeItem(float data)
{
	write7Bit(encodef32(data));
}

void Encoder::writeItem(double data)
{
	write7Bit(encodef64(data));
}

size_t Encoder::encode7Bit(char* buffer, uint64 value)
{
	//外部确保buff足够，int32最多5位，int64最多10位
	//高位标识：0表示结尾,1表示后边还有数据
	size_t count = 0;
	while (value > 0x7F)
	{
		buffer[count++] = ((uint8_t)(value)& 0x7F) | 0x80;
		value >>= 7;
	}
	buffer[count++] = value & 0x7F;
	return count;
}

size_t Encoder::encodeTag(char* buffer, size_t tagOff, uint64 value, bool extra)
{
	// 头编码:head+body: type(1bit)|tag(2bit)|data(5bit) + [tag]+[data]
	// ext(1bit): 0:uint64,1:length+body
	// tag(2bit): 3:表示还有后边还有数据
	// val(5bit): 存储数据的低4位，高位0标识结束，1标识还有数据

	assert(tagOff > 0);
	char flag = extra ? 0x80 : 0;

	// format tag bits
	tagOff -= 1;
	if (tagOff < 3)
	{
		flag |= (char)(tagOff << 5);
		tagOff = 0;
	}
	else
	{
		flag |= 0x60;
		tagOff -= 2;
	}

	// format value bits
	flag |= (value & 0x0F);
	value >>= 4;
	if (value > 0)
		flag |= 0x10;

	// 写入数据
	size_t length = 1;
	buffer[0] = flag;

	if (tagOff > 0)
		length += encode7Bit(buffer + length, tagOff);

	if (value > 0)
		length += encode7Bit(buffer + length, value);

	return length;
}

CUTE_NS_END
