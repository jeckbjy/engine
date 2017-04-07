//! Collection
#pragma once
#include "Cute/Foundation.h"
#include "Cute/Exception.h"
#include <algorithm>

CUTE_NS_BEGIN

/// STL container like C-style array replacement class. 
/// 
/// This implementation is based on the idea of Nicolai Josuttis.
/// His original implementation can be found at http://www.josuttis.com/cppcode/array.html . 
template<class T, std::size_t N>
class Array
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

	enum { static_size = N };

	iterator begin()
	{
		return elems;
	}

	const_iterator begin() const
	{
		return elems;
	}

	iterator end()
	{
		return elems + N;
	}

	const_iterator end() const
	{
		return elems + N;
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
		cute_assert(i < N && "out of range");
		return elems[i];
	}

	const_reference operator[](size_type i) const
	{
		cute_assert(i < N && "out of range");
		return elems[i];
	}

	reference at(size_type i)
	{
		if (i >= size())
			throw InvalidArgumentException("Array::at() range check failed: index is over range");
		return elems[i];
	}

	const_reference at(size_type i) const
	{
		if (i >= size())
			throw InvalidArgumentException("Array::at() range check failed: index is over range");
		return elems[i];
	}

	reference front()
	{
		return elems[0];
	}

	const_reference front() const
	{
		return elems[0];
	}

	reference back()
	{
		return elems[N - 1];
	}

	const_reference back() const
	{
		return elems[N - 1];
	}

	static size_type size()
	{
		return N;
	}

	static bool empty()
	{
		return false;
	}

	static size_type max_size()
	{
		return N;
	}

	void swap(Array<T, N>& y) {
		std::swap_ranges(begin(), end(), y.begin());
	}

	const T* data() const
	{
		return elems;
	}

	T* data()
	{
		return elems;
	}

	T* c_array(){
		return elems;
	}

	template <typename Other>
	Array<T, N>& operator= (const Array<Other, N>& rhs)
	{
		std::copy(rhs.begin(), rhs.end(), begin());
		return *this;
	}

	void assign(const T& value)
	{
		std::fill_n(begin(), size(), value);
	}

public:
	T elems[N];
};

// comparisons
template<class T, std::size_t N>
bool operator== (const Array<T, N>& x, const Array<T, N>& y)
{
	return std::equal(x.begin(), x.end(), y.begin());
}

template<class T, std::size_t N>
bool operator< (const Array<T, N>& x, const Array<T, N>& y)
{
	return std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template<class T, std::size_t N>
bool operator!= (const Array<T, N>& x, const Array<T, N>& y)
{
	return !(x == y);
}

template<class T, std::size_t N>
bool operator> (const Array<T, N>& x, const Array<T, N>& y)
{
	return y < x;
}

template<class T, std::size_t N>
bool operator<= (const Array<T, N>& x, const Array<T, N>& y)
{
	return !(y < x);
}

template<class T, std::size_t N>
bool operator>= (const Array<T, N>& x, const Array<T, N>& y)
{
	return !(x < y);
}

template<class T, std::size_t N>
inline void swap(Array<T, N>& x, Array<T, N>& y)
{
	x.swap(y);
}

CUTE_NS_END