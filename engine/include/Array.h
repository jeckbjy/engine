#pragma once
#include "Platform.h"

CU_NS_BEGIN

// 默认定长,0:时不定长
template<typename T, int N>
class ArrayBase
{
public:
	ArrayBase(){}
	~ArrayBase(){}

	void resize(size_t len) { assert(false); }
	size_t size() const { return N; }

protected:
	T m_data[N];
};

// 0
template<typename T>
class ArrayBase<T, 0>
{
public:
	ArrayBase() :m_data(0), m_size(0){}
	~ArrayBase(){ CU_DELETE(m_data); }

	void resize(size_t len)
	{
		CU_DELETE(m_data);
		if (len > 0)
			m_data = new T[len];
		m_size = len;
	}
	size_t size() const { return m_size; }

protected:
	T*		m_data;
	size_t	m_size;
};

template<typename T, int N = 0>
class Array : public ArrayBase<T, N>
{
public:
	Array() :ArrayBase(){}

	T& operator[](size_t index) { return m_data[index]; }
	const T& operator[](size_t index) const { return m_data[index]; }
};

CU_NS_END
