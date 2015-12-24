#pragma once
#include "API.h"
#include "Buffer.h"

CU_NS_BEGIN

// 自动释放的ptr,谨慎使用
template<typename T>
struct pt_ptr
{
public:
	pt_ptr() :m_ptr(0){}
	~pt_ptr(){ release(); }

	operator bool() const { return m_ptr; }
	bool operator!() const { return !m_ptr; }

	T* operator->() const { return m_ptr; }
	T& operator*() const { return *m_ptr; }
	pt_ptr& operator=(T* x) { release(); m_ptr = x; return *this; }
	void reset() { m_ptr = 0; }
	void release()
	{
		if (m_ptr)
		{
			delete m_ptr;
			m_ptr = 0;
		}
	}
private:
	T* m_ptr;
};

// 默认初始化为0
template<typename T>
struct pt_num
{
	T data;
	pt_num(const T& x = 0) :data(x){}
	operator T() const { return data; }
	pt_num& operator=(const T& x) { data = x; return *this; }
	pt_num& operator++() { ++data; return *this; }
	pt_num& operator--() { --data; return *this; }
	pt_num& operator+=(T x) { data += x; return *this; }
	pt_num& operator-=(T x) { data -= x; return *this; }
	pt_num& operator*=(T x) { data *= x; return *this; }
	pt_num& operator/=(T x) { assert(x != 0); data /= x; return *this; }


	pt_num operator +(T x) { pt_num copy(*this); data += x; return copy; }
	pt_num operator -(T x) { pt_num copy(*this); data -= x; return copy; }
	pt_num operator *(T x) { pt_num copy(*this); data *= x; return copy; }
	pt_num operator /(T x) { pt_num copy(*this); data /= x; return copy; }

	pt_num operator++(int) { pt_num copy(*this); ++data; return copy; }
	pt_num operator--(int) { pt_num copy(*this); --data; return copy; }
};

// 不使用命名空间proto type
typedef pt_num<bool>				pt_bool;
typedef pt_num<signed char>			pt_s8;
typedef pt_num<short>				pt_s16;
typedef pt_num<int>					pt_s32;
typedef pt_num<long long>			pt_s64;
typedef pt_num<unsigned char>		pt_u8;
typedef pt_num<unsigned short>		pt_u16;
typedef pt_num<unsigned int>		pt_u32;
typedef pt_num<unsigned long long>	pt_u64;
typedef pt_num<float>				pt_f32;
typedef pt_num<double>				pt_f64;
typedef pt_num<int>					pt_sint;
typedef pt_num<unsigned int>		pt_uint;
typedef std::string					pt_str;

class ProtoEncoder;
class ProtoDecoder;
struct ICodec
{
	virtual ~ICodec(){}
	virtual void encode(ProtoEncoder& stream) const = 0;
	virtual void decode(ProtoDecoder& stream) = 0;
};

struct IPacket : ICodec
{
	virtual ~IPacket(){}
	virtual size_t msgid() const = 0;
};

class CU_API ProtoCodec
{
public:
	ProtoCodec(Buffer* stream):m_stream(stream), m_tag(0){}

	void beg_tag(size_t& tag) { tag = m_tag; m_tag = 0; }
	void end_tag(size_t& tag) { m_tag = tag; }

protected:
	Buffer* m_stream;
	size_t	m_tag;
};

class CU_API ProtoEncoder
{
public:
	ProtoEncoder(Buffer* stream);

private:
};

class CU_API ProtoDecoder
{
public:
	ProtoDecoder(Buffer* stream);
	~ProtoDecoder();

private:
};

CU_NS_END