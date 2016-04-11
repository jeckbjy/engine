// module: container
#pragma once
#include "API.h"

CU_NS_BEGIN

// VectorImpl
// 有序的vector,可以二分查找
template<typename T>
class SortedVector
{
public:
	SortedVector();
	~SortedVector();

	SortedVector(const SortedVector& rhs);
	SortedVector& operator=(const SortedVector& rhs);

	void resize(size_t count);
	void reserve(size_t count);

	void reverse();
	void clear();
	bool empty() const;

	size_t size() const;
	size_t capacity() const;

	ssize_t indexOf(const T& item) const;

	T& at(size_t index) const;
	T& operator[](size_t index);

	const T& front() const;
	const T& back() const;

	const T& at(size_t index) const;
	const T& operator[](size_t index) const;

	void add(const T& item);
	void remove(const T& item);
	void removeAt(size_t index, size_t count = 1);
	void replace(size_t index, const T& item);

private:
	T*		m_data;
	size_t	m_size;
	size_t	m_capacity;
};

CU_NS_END
