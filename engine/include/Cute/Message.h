//! Server
#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN
//////////////////////////////////////////////////////////////////////////
// Proto Type define
//////////////////////////////////////////////////////////////////////////
// stl
#ifdef CUTE_CPP11
template<typename T>
using pt_vec = std::vector<T>;

template<typename T>
using pt_list = std::list<T>;

template<typename U, typename V>
using pt_map = std::map<U, V>;

template<typename T>
using pt_set = std::set<T>;

template<typename U, typename V>
using pt_hmap = std::unordered_map<U, V>;

template<typename T>
using pt_hset = std::unordered_set<T>;

template<typename U, typename V>
using pt_pair = std::pair<U, V>;

#else
template<typename T>
class pt_vec : public std::vector<T> {};

template<typename T>
class pt_list : public std::list<T>{};

template<typename U, typename V>
class pt_map : public std::map<U, V>{};

template<typename T>
class pt_set : public std::set<T>{};

template<typename U, typename V>
class pt_hmap : public std::hash_map<U, V>{};

template<typename T>
class pt_hset : public std::hash_set<T>{};

template<typename U, typename V>
class pt_pair : public std::pair<U, V> {};

#endif

template<typename T, T val>
struct pt_constant
{
	static const T value = val;
	operator T() const { return value; }
};

typedef pt_constant<bool, true>		pt_true_type;
typedef pt_constant<bool, false>	pt_false_type;

template<typename T>
struct pt_is_basic : pt_false_type { };
template<> struct pt_is_basic<bool> : pt_true_type{};
template<> struct pt_is_basic<char> : pt_true_type{};
template<> struct pt_is_basic<int8_t> : pt_true_type{};
template<> struct pt_is_basic<int16_t> : pt_true_type{};
template<> struct pt_is_basic<int32_t> : pt_true_type{};
template<> struct pt_is_basic<int64_t> : pt_true_type{};
template<> struct pt_is_basic<uint8_t> : pt_true_type{};
template<> struct pt_is_basic<uint16_t> : pt_true_type{};
template<> struct pt_is_basic<uint32_t> : pt_true_type{};
template<> struct pt_is_basic<uint64_t> : pt_true_type{};
template<> struct pt_is_basic<float> : pt_true_type{};
template<> struct pt_is_basic<double> : pt_true_type{};

template<typename T>
struct pt_is_stl : pt_false_type { };
template<typename T> struct pt_is_stl<pt_vec<T> > : pt_true_type{};
template<typename T> struct pt_is_stl<pt_list<T> > : pt_true_type{};
template<typename T> struct pt_is_stl<pt_set<T> > : pt_true_type{};
template<typename T> struct pt_is_stl<pt_hset<T> > : pt_true_type{};
template<typename U, typename V> struct pt_is_stl<pt_map<U, V> > : pt_true_type{};
template<typename U, typename V> struct pt_is_stl<pt_hmap<U, V> > : pt_true_type{};

// traits 萃取
template<bool _Test, class T = void>
struct pt_enable_if{};

template<class T>
struct pt_enable_if<true, T> { typedef T type; };

// stl helper
template<typename T>
inline void pt_push(pt_vec<T>& stl, typename pt_vec<T>::value_type& t)
{
	stl.push_back(t);
}
template<typename T>
inline void pt_push(pt_list<T>& stl, typename pt_list<T>::value_type& t)
{
	stl.push_back(t);
}
template<typename T>
inline void pt_push(pt_set<T>& stl, typename pt_set<T>::value_type& t)
{
	stl.insert(t);
}
template<typename T>
inline void pt_push(pt_hset<T>& stl, typename pt_hset<T>::value_type& t)
{
	stl.insert(t);
}

template<typename U, typename V>
inline void pt_push(pt_map<U, V>& stl, typename pt_map<U, V>::value_type& t)
{
	stl.insert(t);
}

template<typename U, typename V>
inline void pt_push(pt_hmap<U, V>& stl, typename pt_hmap<U, V>::value_type& t)
{
	stl.insert(t);
}

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
	T& operator*() const { return *get(); }

	pt_ptr& operator=(T* x)
	{
		release();
		m_ptr = x;
		return *this;
	}

	void reset()
	{
		m_ptr = 0;
	}

	void release()
	{
		if (m_ptr)
		{
			delete m_ptr;
			m_ptr = 0;
		}
	}

	const T* get() const
	{
		return m_ptr;
	}

	T* get()
	{
		if (m_ptr == 0)
			m_ptr = new T();

		return m_ptr;
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
	pt_num operator-() { return pt_num(-data); }
};

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
typedef String						pt_str;

class Encoder;
class Decoder;
// 可以指定一个非零唯一ID
class Message
{
public:
	virtual ~Message(){}
	virtual size_t msgid() const { return 0; }
	virtual void encode(Encoder& stream) { }
	virtual void decode(Decoder& stream) { }
};

template<int TMSGID>
class TMessage : public Message
{
public:
	enum { MSG_ID = TMSGID };
	size_t msgid() const { return MSG_ID; }
};

#define AUTO_CODEC(fields)								\
	void encode(Encoder& stream) { stream & fields; }	\
	void decode(Decoder& stream) { stream & fields; }

/*
Examples:

enum MsgID
{
MSGID_REQ_LOGIN,
MSGID_REQ_AUTH,

MSGID_RSP_LOGIN,
MSGID_RSP_AUTH,
};

struct LoginReq : public TMessage<MSGID_REQ_LOGIN>
{
pt_str account;
pt_str password;

void encode(Encoder& stream)
{
stream << account << password;
}

void decode(Decoder& stream)
{
stream >> account >> password;
}
};

struct LoginRsp : public TMessage<MSGID_RSP_LOGIN>
{
pt_bool result;

void encode(Encoder& stream)
{
stream << result;
}

void decode(Decoder& stream)
{
stream >> result;
}
};

struct AuthReq : public TMessage<MSGID_REQ_AUTH>
{
pt_u64 uid;
pt_u64 secret;

AUTO_CODEC( uid & secret);
};

struct AuthRsp : public TMessage<MSGID_RSP_AUTH>
{
pt_bool result;

AUTO_CODEC( result );
};
*/

CUTE_NS_END
