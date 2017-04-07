#pragma once
#include "Cute/Foundation.h"
#include "Cute/Message.h"
#include "Cute/BufferList.h"

CUTE_NS_BEGIN

// like TLV(type-length-value) encode
// field = head + body
// head = flag(1byte) + (tag extend) + (data extend)
// flag:1bit��ʶ�Ƿ��ж�������,2bit��ʶtag offset��5bit��ʶС��data
// Packet:����,
// ����Ҫ��tag����������ɲ�����
// �䳤field����:
class Message;
class CUTE_CORE_API Encoder
{
	typedef BufferList Stream;
public:
	Encoder(Stream* stream);
	~Encoder();

	size_t encode(Message* msg);

	// ͨ��tagд��
	template<typename T>
	Encoder& write(const T& data, size_t tag);

	// ͨ��tagд��
	template<typename T>
	Encoder& write2(const T& data, size_t off);

	// ����ʽд�룬ƫ��Ϊ1
	template<typename T>
	Encoder& operator << (const T& data);

	// ����ʽд�룬ƫ��Ϊ1
	template<typename T>
	Encoder& operator &  (const T& data);

public:
	void skip(size_t offset);
	void write7Bit(uint64 data);
	void writeData(const char* data, size_t length);

	void writeTag(size_t tagOff, uint64 value, bool extra);
	void writeBeg(size_t& spos);
	void writeEnd(size_t spos);

	bool writeField(Message& msg);
	bool writeField(const String& str);
	bool writeField(int8 data);
	bool writeField(int16 data);
	bool writeField(int32 data);
	bool writeField(int64 data);
	bool writeField(uint8 data);
	bool writeField(uint16 data);
	bool writeField(uint32 data);
	bool writeField(uint64 data);
	bool writeField(float data);
	bool writeField(double data);

	template<typename T>
	bool writeField(const pt_num<T>& num);

	template<typename T>
	bool writeField(const pt_ptr<T>& ptr);

	template<typename STL>
	typename pt_enable_if<pt_is_stl<STL>::value, bool>::type
		writeField(const STL& data);

public:
	void writeItem(Message& msg);
	void writeItem(const String& str);
	void writeItem(int8 data);
	void writeItem(int16 data);
	void writeItem(int32 data);
	void writeItem(int64 data);
	void writeItem(uint8 data);
	void writeItem(uint16 data);
	void writeItem(uint32 data);
	void writeItem(uint64 data);
	void writeItem(float data);
	void writeItem(double data);

private:
	uint64 encodei64(int64 n){ return (n << 1) ^ (n >> 63); }
	uint64 encodef64(double n) { union { double f; uint64 i; } d; d.f = n; return d.i; }
	uint64 encodef32(float n)  { union { float  f; uint32 i; } d; d.f = n; return d.i; }
	size_t encode7Bit(char* buffer, uint64 value);
	size_t encodeTag(char* buffer, size_t tagOff, uint64 value, bool extra);

private:
	Stream* m_stream;
	size_t	m_spos;		// ��ʼ��
	size_t	m_tag;		// ��ǰӦд���ʶ
	size_t	m_off;		// ������ϴ�д��ƫ��
};

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
template<typename T>
Encoder& Encoder::write(const T& data, size_t tag)
{
	assert(tag > m_tag);
	return write2(data, tag - m_tag);
}

template<typename T>
Encoder& Encoder::write2(const T& data, size_t off)
{
	m_tag += off;
	m_off += off;
	if (writeField(data))
	{
		m_off = 0;
	}

	return *this;
}

template<typename T>
Encoder& Encoder::operator << (const T& data)
{
	return write2(data, 1);
}

template<typename T>
Encoder& Encoder::operator & (const T& data)
{
	return write2(data, 1);
}

template<typename T>
bool Encoder::writeField(const pt_num<T>& num)
{
	return writeField(num.data);
}

template<typename T>
bool Encoder::writeField(const pt_ptr<T>& ptr)
{
	if (ptr)
		return writeField(*ptr);
	return false;
}

template<typename STL>
typename pt_enable_if<pt_is_stl<STL>::value, bool>::type
	Encoder::writeField(const STL& data)
{
	if (data.empty())
		return false;

	size_t spos;
	writeBeg(spos);

	typename STL::const_iterator cur_itor;
	typename STL::const_iterator end_itor = data.end();
	for (cur_itor = data.begin(); cur_itor != end_itor; ++cur_itor)
	{
		writeItem(*cur_itor);
	}

	writeEnd(spos);
	return true;
}

CUTE_NS_END
