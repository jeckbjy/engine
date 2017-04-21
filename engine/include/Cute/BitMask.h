//! Collection
#pragma once
#include "Cute/Platform.h"
#include "Cute/Bitwise.h"

CUTE_NS_BEGIN

template<typename T>
class TBitMask
{
	typedef TBitMask<T> this_type;
public:
	TBitMask() :m_data(0){}
	TBitMask(T val) :m_data(val){}

	T data() const { return m_data; }

	void flip();
	void reset();
	void set();
	void set(T mask, bool value);
	bool test(T mask);
	bool any() const;
	bool none() const;
	size_t count() const;

	this_type& operator=(T val)
	{
		m_data = val;
		return *this;
	}

	bool operator == (const this_type& other) const
	{
		return m_data == other.m_data;
	}

	bool operator != (const this_type& other) const
	{
		return m_data != other.m_data;
	}

private:
	T m_data;
};

//////////////////////////////////////////////////////////////////////////
// inline
//////////////////////////////////////////////////////////////////////////
template<typename T>
inline void TBitMask<T>::flip()
{
	m_data = ~m_data;
}

template<typename T>
void TBitMask<T>::set()
{
	m_data = ~((T)0);
}

template<typename T>
void TBitMask<T>::set(T mask, bool value)
{
	if (value)
		m_data |= mask;
	else
		m_data &= ~mask;
}

template<typename T>
void TBitMask<T>::reset()
{
	m_data = 0;
}

template<typename T>
bool TBitMask<T>::test(T mask)
{
	return (m_data & mask) != 0;
}

template<typename T>
bool TBitMask<T>::any() const
{
	return m_data != 0;
}

template<typename T>
bool TBitMask<T>::none() const
{
	return m_data == 0;
}

template<typename T>
size_t TBitMask<T>::count() const
{
	return Bitwise::popcount(m_data);
}

typedef TBitMask<uint8>  BitMask8;
typedef TBitMask<uint16> BitMask16;
typedef TBitMask<uint32> BitMask32;
typedef TBitMask<uint64> BitMask64;

CUTE_NS_END
