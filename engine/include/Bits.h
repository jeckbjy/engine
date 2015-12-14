#pragma once
#include "API.h"

CU_NS_BEGIN

namespace detail
{
	template<typename T, bool Mode>
	struct ToMask;
	template<typename T> struct ToMask<T, true>
	{
		inline static T mask(const T& t){ return t; }
	};
	template<typename T> struct ToMask<T, false>
	{
		inline static T mask(const T& t) { return 1 << t; }
	};

}

// MaskMode是否使用掩码作为索引
template<typename T, bool IsMask>
class TBits
{
public:
	typedef T type_t;

	class reference
	{
		friend class TBits<T, IsMask>;
	private:
		T& m_value;
		const T m_pos;
		reference(T& v, T pos) :m_value(v), m_pos(pos){}
		void operator&();

	public:
		operator bool() const { return (m_value & mask()) != 0; }
		bool operator~() const { return (m_value & mask()) == 0; }

		reference& operator=(bool x) { assign(x); return *this; }
		reference& operator=(const reference& rhs){ assign(rhs); return *this; }

	private:
		inline T mask() const { return detail::ToMask<T, IsMask>::mask(m_pos); }
		void assign(bool flag)
		{
			if (flag)
				m_value |= mask();
			else
				m_value &= ~mask();
		}
	};

private:
	T m_value;

	inline T mask(T pos) const { return detail::ToMask<T, IsMask>::mask(pos); }

public:
	TBits() :m_value(0){}
	TBits(T value) :m_value(value){}

	inline T data() const { return m_value; }
	inline T& data() { return m_value; }

	inline bool empty() const { return m_value == 0; }
	inline void clear() { m_value = 0; }
	inline void flip(T pos) { set(pos, !get(pos)); }

	inline bool get(T pos) const { return (m_value & mask(pos)) != 0; }
	inline void set(T pos, bool flag = true)
	{
		if (flag)
			m_value |= mask(pos);
		else
			m_value &= ~mask(pos);
	}
	inline void reset(T pos){ m_value &= ~mask(pos); }

public:
	inline operator T() const { return m_value; }
	inline bool operator[](unsigned pos) const { return get(pos); }
	inline reference operator[](unsigned pos) { return reference(m_value, pos); }
};

template<typename T>
class TFlag
{
public:
	typedef T type_t;

	struct holder
	{
		T& val;
		T  pos;
		holder(T& v, T pos) :val(v), pos(pos){}
		holder& operator=(bool flag)
		{
			T mask = 1 << pos;
			val = flag ? (val | mask) : val & (~mask);
		}
	};
public:
	TFlag(T v = 0) :m_value(v){}

	inline void clear() { m_value = 0; }
	inline void flip(T pos) { set(pos, !get(pos)); }
	inline void get(T pos) const { return (m_value & (1 << pos)) != 0; }
	inline void set(T pos, bool flag)
	{
		if (flag)
			m_value |= 1 << pos;
		else
			m_value &= ~(1 << pos);
	}

	inline bool operator[](T index) const { return m_value & (1 << index); }
	inline holder operator[](T index) { return holder(m_value, index); }

	inline operator T() const { return m_value; }

private:
	T m_value;
};

// 直接使用掩码操作
template<typename T>
class TMask
{
public:
	typename T type_t;
	struct holder
	{
		T& data;
		T  mask;
		holder(T& v, T m) :data(v), mask(m){}
	};
public:
	TMask(T v = 0) :m_value(v){}

private:
	T m_value;
};

//////////////////////////////////////////////////////////////////////////
// 每个值占有N位的bits数组,如每个占3bit，存储于uint16中，可以存储5个值
//////////////////////////////////////////////////////////////////////////
template<typename T, int N = 1>
class BitsArray
{
public:
	typedef T type_t;
	static const T MASK = (1 << (N + 1)) - 1;	// 掩码

	class reference
	{
		friend class BitsArray<T, N>;
	private:
		T& m_value;
		unsigned m_index;
		reference(T& v, unsigned index) :m_value(v), m_index(index){}

	public:
		inline operator T() const { return (m_value >> (m_index * N)) & MASK; }
		inline reference& operator=(T x) { assign(x); return *this; }
		inline reference& operator=(const reference& rhs){ assign(rhs.m_value); return *this; }

	private:
		void assign(T data)
		{
			assert((data & (~MASK)) == 0);
			unsigned offset = m_index * N;
			m_value &= ~(MASK << offset);
			m_value |= data << offset;
		}
	};

public:
	BitsArray() :m_value(0){}
	BitsArray(T t) :m_value(t){}
	inline T data() const { return m_value; }
	inline T& data() { return m_value; }

	inline bool empty() const { return m_value == 0; }
	inline void clear() { m_value = 0; }

	inline T get(unsigned index) const { return (m_value >> (index * N)) & MASK; }
	inline void set(unsigned index, T value)
	{
		assert((value & (~MASK)) == 0);
		// clear first
		unsigned offset = index * N;
		m_value &= ~(MASK << offset);
		m_value |= value << offset;
	}

	inline operator T() const { return m_value; }
	inline T operator[](unsigned index) const { return get(index); }
	inline reference operator[](unsigned index){ return reference(m_value, index); }

private:
	T m_value;
};

// index mode
typedef TBits<uint8_t, false>	Flag8;
typedef TBits<uint16_t, false>	Flag16;
typedef TBits<uint32_t, false>	Flag32;
typedef TBits<uint64_t, false>	Flag64;

// mask mode
typedef TBits<uint8_t, true>	Mask8;
typedef TBits<uint16_t, true>	Mask16;
typedef TBits<uint32_t, true>	Mask32;
typedef TBits<uint64_t, true>	Mask64;

CU_NS_END