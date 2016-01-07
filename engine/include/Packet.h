#pragma once
#include "API.h"
#include "Buffer.h"

CU_NS_BEGIN

// stl
#ifdef CU_CPP11
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

// stl helper
template<typename T>
inline void pt_push_back(pt_vec<T>& stl, typename pt_vec<T>::value_type& t)
{
	stl.push_back(t);
}
template<typename T>
inline void pt_push_back(pt_list<T>& stl, typename pt_list<T>::value_type& t)
{
	stl.push_back(t);
}
template<typename T>
inline void pt_push_back(pt_set<T>& stl, typename pt_set<T>::value_type& t)
{
	stl.insert(t);
}
template<typename T>
inline void pt_push_back(pt_hset<T>& stl, typename pt_hset<T>::value_type& t)
{
	stl.insert(t);
}

template<typename U, typename V>
inline void pt_push_back(pt_map<U, V>& stl, typename pt_map<U, V>::value_type& t)
{
	stl.insert(t);
}

template<typename U, typename V>
inline void pt_push_back(pt_hmap<U, V>& stl, typename pt_hmap<U, V>::value_type& t)
{
	stl.insert(t);
}

// traits 萃取
template<bool _Test, class T = void>
struct pt_enable_if{};

template<class T>
struct pt_enable_if<true, T> { typedef T type; };

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

class pt_encoder;
class pt_decoder;
struct pt_message
{
	virtual ~pt_message(){}
	virtual uint msgid() const { return 0; }
	virtual void encode(pt_encoder& stream) const = 0;
	virtual void decode(pt_decoder& stream) = 0;
};

template<int TMSG_ID>
struct TPacket : pt_message
{
	enum { MSG_ID = TMSG_ID, };
	size_t msgid() const { return MSG_ID; }
};

struct pt_convert
{
	inline static uint64_t encodei64(int64_t n){ return (n << 1) ^ (n >> 63); }
	inline static uint64_t encodef64(double n) { union { double f; uint64_t i; } d; d.f = n; return d.i; }
	inline static uint64_t encodef32(float n)  { union { float  f; uint32_t i; } d; d.f = n; return d.i; }

	inline static int64_t  decodei64(uint64_t n) { return (n >> 1) ^ -(int64_t)(n & 1); }
	inline static double   decodef64(uint64_t n) { union { double f; uint64_t i; } d; d.i = n; return d.f; }
	inline static float    decodef32(uint64_t n) { union { float  f; uint32_t i; } d; d.i = (uint32_t)n; return d.f; }

	inline static uint64_t encode(const bool& n)	 { return n ? 1 : 0; }
	inline static uint64_t encode(const uint8_t&  n) { return n; }
	inline static uint64_t encode(const uint16_t& n) { return n; }
	inline static uint64_t encode(const uint32_t& n) { return n; }
	inline static uint64_t encode(const uint64_t& n) { return n; }
	inline static uint64_t encode(const int8_t&  n)  { return encodei64(n); }
	inline static uint64_t encode(const int16_t& n)  { return encodei64(n); }
	inline static uint64_t encode(const int32_t& n)  { return encodei64(n); }
	inline static uint64_t encode(const int64_t& n)  { return encodei64(n); }
	inline static uint64_t encode(const double& n)	 { return encodef64(n); }
	inline static uint64_t encode(const float& n)	 { return encodef32(n); }

	inline static void decode(uint64_t n, bool& dst)	{ dst = (n != 0); }
	inline static void decode(uint64_t n, uint8_t& dst)	{ dst = (uint8_t)n; }
	inline static void decode(uint64_t n, uint16_t& dst){ dst = (uint16_t)n; }
	inline static void decode(uint64_t n, uint32_t& dst){ dst = (uint32_t)n; }
	inline static void decode(uint64_t n, uint64_t& dst){ dst = (uint64_t)n; }
	inline static void decode(uint64_t n, int8_t& dst)	{ dst = (int8_t)decodei64(n); }
	inline static void decode(uint64_t n, int16_t& dst)	{ dst = (int16_t)decodei64(n); }
	inline static void decode(uint64_t n, int32_t& dst)	{ dst = (int32_t)decodei64(n); }
	inline static void decode(uint64_t n, int64_t& dst)	{ dst = (int64_t)decodei64(n); }
	inline static void decode(uint64_t n, double& dst)	{ dst = decodef64(n); }
	inline static void decode(uint64_t n, float& dst)	{ dst = decodef32(n); }
};

class pt_encoder
{
public:
	pt_encoder(Buffer* stream) :m_tag(0), m_stream(stream){}

	// 顶层packet
	void encode(pt_message& msg);

	void write_msg(const pt_message& msg);
	void write_buf(const char* buf, size_t len);
	void write_var(uint64_t data);
	void write_beg(size_t& index, size_t tag);
	void write_end(size_t& index);
	void write_tag(size_t tag, uint64_t val);

public:
	bool write_field(const pt_message& msg, size_t tag);
	bool write_field(const pt_str& str, size_t tag);
	template<typename T>
	typename pt_enable_if<pt_is_basic<T>::value, bool>::type
		write_field(const T& data, size_t tag)
	{
		uint64_t tmp = pt_convert::encode(data);
		if (tmp == 0)
			return false;
		write_tag(tag, tmp);
	}

	template<typename STL>
	typename pt_enable_if<pt_is_stl<STL>::value, bool>::type
		write_field(const STL& data, size_t tag)
	{
		size_t index;
		write_beg(index, tag);
		// 写入数据
		typename STL::const_iterator cur_itor;
		typename STL::const_iterator end_itor = data.end();
		for (cur_itor = data.begin(); cur_itor != end_itor; ++cur_itor)
		{
			write_item(*cur_itor);
		}
		write_end(index);
	}
	// wrapper
	template<typename T>
	bool write_field(const pt_ptr<T>& ptr, size_t tag)
	{
		if (!ptr)
			return false;
		return write_field(*ptr, tag);
	}

	template<typename T>
	bool write_field(const pt_num<T>& num, size_t tag)
	{
		return write_field(num.data, tag);
	}

	template<typename T>
	pt_encoder& operator << (const T& data)
	{
		return write(data, 1);
	}

	template<typename T>
	pt_encoder& write(const T& data, size_t tag)
	{
		m_tag += tag;
		if (write_field(data, m_tag))
			m_tag = 0;
		return *this;
	}
public:
	void write_item(const pt_str& str);
	void write_item(const pt_message& msg);
	template<typename T>
	typename pt_enable_if<pt_is_basic<T>::value>::type
		write_item(const T& data)
	{
		uint64_t tmp = pt_convert::encode(data);
		write_var(tmp);
	}

private:
	struct Tag
	{
		size_t tpos;
		size_t bpos;
		size_t leng;
	};
	typedef std::vector<Tag> TagVec;
	size_t	m_tag;
	Buffer* m_stream;
	TagVec	m_indexs;
};

// 解码
class CU_API pt_decoder
{
public:
	pt_decoder(Buffer* stream);

	bool parse();
	bool decode(pt_message& msg);

	bool read_tag(bool use_tag);
	bool pre_read(size_t tag);

	uint suspend(size_t len);
	void recovery(size_t epos);
	bool eof() const;
	bool read_buf(char* buf, size_t len);
	bool read_var(uint64_t& data);
	void read_msg(pt_message& msg);

	uint msgid() const { return m_msgid; }
	uint msglen() const { return m_head_len + m_body_len + m_tail_len; }
	Buffer split();
	void discard() { m_stream->discard(); }

public:
	void read_field(pt_str& str);
	void read_field(pt_message& msg);
	template<typename T>
	void read_field(pt_ptr<T>& ptr)
	{
		ptr = new T();
		read_field(*ptr);
	}

	template<typename T>
	void read_field(pt_num<T>& num)
	{
		pt_convert::decode(m_data, num.data);
	}

	template<typename T>
	typename pt_enable_if<pt_is_basic<T>::value>::type
		read_field(T& num)
	{
		pt_convert::decode(m_data, num);
	}

	template<typename STL>
	typename pt_enable_if<pt_is_stl<STL>::value>::type
		read_field(STL& stl)
	{
		uint epos = suspend();
		typename STL::value_type item;
		while (!eof())
		{
			// read_item
			read_item(item);
			pt_push_back(stl, item);
		}
		recovery(epos);
	}

	template<typename T>
	pt_decoder& operator>>(T& data)
	{
		return read(data, 1);
	}

	template<typename T>
	pt_decoder& read(T& data, size_t tag = 1)
	{
		if (pre_read(tag))
			read_field(data);
		return *this;
	}

public:
	void read_item(pt_str& str);
	void read_item(pt_message& msg);
	template<typename T>
	typename pt_enable_if<pt_is_basic<T>::value>::type
		read_item(T& data)
	{
		pt_convert::decode(m_data, data);
	}

private:
	enum
	{
		T_VAL,	// 数值类型
		T_MSG,	// len+content类型
	};
	typedef std::vector<size_t> SizeVec;
	Buffer* m_stream;
	SizeVec	m_sizes;
	size_t	m_tag;
	size_t	m_epos;			// 当前有效数据结尾
	// read tags temp data
	char	m_type;
	uint64	m_data;
	// read head temp data
	size_t	m_msgid;
	size_t	m_head_len;		// 消息头长度
	size_t	m_body_len;		// 消息体长度,不含index
	size_t	m_tail_len;		// 索引大小
};

CU_NS_END
