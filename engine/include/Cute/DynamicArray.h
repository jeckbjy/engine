//! Collection
#pragma once
#include "Cute/Platform.h"
#include "Cute/Exception.h"
#include <algorithm>

CUTE_NS_BEGIN

/// STL container like C-style array replacement class. 
/// 
/// This implementation is based on the idea of Nicolai Josuttis.
/// His original implementation can be found at http://www.josuttis.com/cppcode/array.html . 
template<class T>
class DynamicArray
{
public:
	typedef T				value_type;
	typedef T*				iterator;
	typedef const T*		const_iterator;
	typedef T&				reference;
	typedef const T&		const_reference;
	typedef std::size_t		size_type;
	typedef std::ptrdiff_t  difference_type;

	typedef std::reverse_iterator<iterator>			reverse_iterator;
	typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;

	DynamicArray()
		: m_elems(NULL), m_size(0)
	{
	}

	~DynamicArray()
	{
		if (m_elems != NULL)
		{
			delete[] m_elems;
			m_elems = NULL;
			m_size = 0;
		}
	}

	void resize(size_t count)
	{
		if (m_size == count)
			return;

		size_t copyCount = std::min(m_size, count);

		T* elems = new T[count];

		if (m_elems != NULL)
		{
			std::copy(m_elems, m_elems + copyCount, elems);
			delete[] m_elems;
		}

		m_elems = elems;
		m_size = count;
	}

	void clear()
	{
		if (m_elems != NULL)
		{
			delete[] m_elems;
			m_elems = NULL;
			m_size = 0;
		}
	}

	iterator begin()
	{
		return m_elems;
	}

	const_iterator begin() const
	{
		return m_elems;
	}

	iterator end()
	{
		return m_elems + m_size;
	}

	const_iterator end() const
	{
		return m_elems + m_size;
	}

	reverse_iterator rbegin()
	{
		return reverse_iterator(end());
	}

	const_reverse_iterator rbegin() const
	{
		return const_reverse_iterator(end());
	}

	reverse_iterator rend()
	{
		return reverse_iterator(begin());
	}

	const_reverse_iterator rend() const
	{
		return const_reverse_iterator(begin());
	}

	reference operator[](size_type i)
	{
		assert(i < m_size && "out of range");
		return m_elems[i];
	}

	const_reference operator[](size_type i) const
	{
		assert(i < m_size && "out of range");
		return m_elems[i];
	}

	reference at(size_type i)
	{
		if (i >= size())
			throw InvalidArgumentException("Array::at() range check failed: index is over range");
		return m_elems[i];
	}

	const_reference at(size_type i) const
	{
		if (i >= size())
			throw InvalidArgumentException("Array::at() range check failed: index is over range");
		return m_elems[i];
	}

	reference front()
	{
		return m_elems[0];
	}

	const_reference front() const
	{
		return m_elems[0];
	}

	reference back()
	{
		return m_elems[m_size - 1];
	}

	const_reference back() const
	{
		return m_elems[m_size - 1];
	}

	size_type size()
	{
		return m_size;
	}

	bool empty()
	{
		return m_size != 0;
	}

	void swap(DynamicArray<T>& y)
	{
		std::swap(m_elems, y.m_elems);
		std::swap(m_size, y.m_size);
	}

	const T* data() const
	{
		return m_elems;
	}

	T* data()
	{
		return m_elems;
	}

	T* c_array()
	{
		return m_elems;
	}

	template <typename Other>
	DynamicArray<T>& operator= (const DynamicArray<Other>& rhs)
	{
		resize(rhs.m_size);
		std::copy(rhs.begin(), rhs.end(), begin());
		return *this;
	}

	void assign(const T& value)
	{
		std::fill_n(begin(), size(), value);
	}

public:
	T*		m_elems;
	size_t	m_size;
};

// comparisons
template<class T>
bool operator== (const DynamicArray<T>& x, const DynamicArray<T>& y)
{
	return (x.m_size == y.m_size) && std::equal(x.begin(), x.end(), y.begin());
}

template<class T>
bool operator!= (const DynamicArray<T>& x, const DynamicArray<T>& y)
{
	return !(x == y);
}

template<class T>
inline void swap(DynamicArray<T>& x, DynamicArray<T>& y)
{
	x.swap(y);
}

CUTE_NS_END
