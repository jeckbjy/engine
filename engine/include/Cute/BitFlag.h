//! Collection
#pragma once
#include "Cute/Platform.h"
#include "Cute/Bitwise.h"

CUTE_NS_BEGIN

template<typename T>
class TBitFlag
{
	typedef TBitFlag<T> this_type;
public:
	TBitFlag() :m_data(0){}
	TBitFlag(T val) :m_data(val){}

	T data() const { return m_data; }

	void flip();
	void reset();
	void set();
	void set(size_t index, bool value);
	bool test(size_t index);
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
inline void TBitFlag<T>::flip()
{
	m_data = ~m_data;
}

template<typename T>
void TBitFlag<T>::set()
{
	m_data = ~((T)0);
}

template<typename T>
void TBitFlag<T>::set(size_t i, bool value)
{
	if (value)
		m_data |= (((T)1) << i);
	else
		m_data &= ~(((T)1) << i);
}

template<typename T>
void TBitFlag<T>::reset()
{
	m_data = 0;
}

template<typename T>
bool TBitFlag<T>::test(size_t index)
{
	return (m_data & ((T)1 << index)) != 0;
}

template<typename T>
bool TBitFlag<T>::any() const
{
	return m_data != 0;
}

template<typename T>
bool TBitFlag<T>::none() const
{
	return m_data == 0;
}

template<typename T>
size_t TBitFlag<T>::count() const
{
	return Bitwise::popcount(m_data);
}

typedef TBitFlag<uint8>  BitFlag8;
typedef TBitFlag<uint16> BitFlag16;
typedef TBitFlag<uint32> BitFlag32;
typedef TBitFlag<uint64> BitFlag64;

CUTE_NS_END
