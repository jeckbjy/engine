#pragma once
#include "Cute/Foundation.h"
#include "Cute/Message.h"
#include "Cute/BufferList.h"

CUTE_NS_BEGIN

// 消息解析
class CUTE_CORE_API Decoder
{
	typedef BufferList Stream;
public:
	Decoder(Stream* stream);
	Decoder(Stream* stream, size_t length);
	~Decoder();

	bool decode(Message* msg);

	// 通过tag读取
	template<typename T>
	Decoder& read(T& data, size_t tag);

	// 通过off读取
	template<typename T>
	Decoder& read2(T& data, size_t off);

	// 流方式读取，偏移为1
	template<typename T>
	Decoder& operator >> (T& data);

	// 流方式读取，偏移为1
	template<typename T>
	Decoder& operator &  (T& data);

public:
	uint64 read7Bit();

	bool eof() const;

	bool readTag();
	bool read7Bit(uint64& data);
	bool readData(char* buffer, size_t length);
	// 通过tag读取field的头部信息
	bool readHead(size_t tag);

	void readField(Message& msg);
	void readField(String& str);
	void readField(int8& data);
	void readField(int16& data);
	void readField(int32& data);
	void readField(int64& data);
	void readField(uint8& data);
	void readField(uint16& data);
	void readField(uint32& data);
	void readField(uint64& data);
	void readField(float& data);
	void readField(double& data);

	template<typename T>
	bool readField(pt_num<T>& num);

	template<typename T>
	bool readField(pt_ptr<T>& ptr);

	template<typename STL>
	typename pt_enable_if<pt_is_stl<STL>::value, bool>::type
		readField(STL& stl);

public:
	void readItem(Message& data);
	void readItem(String& data);
	void readItem(int8& data);
	void readItem(int16& data);
	void readItem(int32& data);
	void readItem(int64& data);
	void readItem(uint8& data);
	void readItem(uint16& data);
	void readItem(uint32& data);
	void readItem(uint64& data);
	void readItem(float& data);
	void readItem(double& data);

private:
	int64	decodei64(uint64 n) { return (n >> 1) ^ -(int64)(n & 1); }
	double	decodef64(uint64 n) { union { double f; uint64 i; } d; d.i = n; return d.f; }
	float	decodef32(uint64 n) { union { float  f; uint32 i; } d; d.i = (uint32)n; return d.f; }

private:
	Stream*		m_stream;
	size_t		m_spos;
	size_t		m_epos;
	size_t		m_hint;		// 偏移方式计算tag
	// head data
	size_t		m_tag;		// tag值>0(field tag：不是偏移值)
	uint64		m_value;
	bool		m_extra;	// 是否含有额外数据
};

//////////////////////////////////////////////////////////////////////////
// template
//////////////////////////////////////////////////////////////////////////
template<typename T>
Decoder& Decoder::read(T& data, size_t tag)
{
	assert(tag >= m_hint);
	m_hint = tag;
	if (readHead(tag))
	{
		size_t epos = 0;
		if (m_extra)
			epos = m_stream->position() + (size_t)m_value;

		readField(data);

		// 确保每个field都正取读取完
		if (m_extra)
		{
			m_stream->seek(epos, SEEK_SET);
			m_value = 0;
		}
	}

	return *this;
}

template<typename T>
Decoder& Decoder::read2(T& data, size_t off)
{
	m_hint += off;
	return read(data, m_hint);
}

template<typename T>
Decoder& Decoder::operator >>(T& data)
{
	return read2(data, 1);
}

template<typename T>
Decoder& Decoder::operator &  (T& data)
{
	return read2(data, 1);
}

template<typename T>
bool Decoder::readField(pt_num<T>& num)
{
	return readField(num.data);
}

template<typename T>
bool Decoder::readField(pt_ptr<T>& ptr)
{
	return readField(*ptr);
}

template<typename STL>
typename pt_enable_if<pt_is_stl<STL>::value, bool>::type
	Decoder::readField(STL& stl)
{
	typename STL::value_type item;

	Decoder decoder(m_stream, (size_t)m_value);

	while (!decoder.eof())
	{
		decoder.readItem(item);
		pt_push(stl, item);
	}
}

CUTE_NS_END
